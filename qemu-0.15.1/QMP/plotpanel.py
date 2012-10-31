#from camera import *
from numpy import *
import threading
import matplotlib.pyplot as plt
import time
import wx
import wx.lib.plot as plot
from wx.lib.pubsub import Publisher as pub
import numpy as np
import os
from wx.lib.pubsub import Publisher as pub


#!/usr/bin/python
#
# Low-level QEMU shell on top of QMP.
#
# Copyright (C) 2009, 2010 Red Hat Inc.
#
# Authors:
#  Luiz Capitulino <lcapitulino@redhat.com>
#
# This work is licensed under the terms of the GNU GPL, version 2.  See
# the COPYING file in the top-level directory.
#
# Usage:
#
# Start QEMU with:
#
# # qemu [...] -qmp unix:./qmp-sock,server
#
# Run the shell:
#
# $ qmp-shell ./qmp-sock
#
# Commands have the following format:
#
#    < command-name > [ arg-name1=arg1 ] ... [ arg-nameN=argN ]
#
# For example:
#
# (QEMU) device_add driver=e1000 id=net1
# {u'return': {}}
# (QEMU)

import qmp
import readline
import sys
import threading
import time
import json
import mutex



class QMPCompleter(list):
    def complete(self, text, state):
        for cmd in self:
            if cmd.startswith(text):
                if not state:
                    return cmd
                else:
                    state -= 1

class QMPShellError(Exception):
    pass

class QMPShellBadPort(QMPShellError):
    pass

# TODO: QMPShell's interface is a bit ugly (eg. _fill_completion() and
#       _execute_cmd()). Let's design a better one.
class QMPShell(qmp.QEMUMonitorProtocol):
    def __init__(self, address):
        qmp.QEMUMonitorProtocol.__init__(self, self.__get_address(address))
        self._greeting = None
        self._completer = None

    def __get_address(self, arg):
        """
        Figure out if the argument is in the port:host form, if it's not it's
        probably a file path.
        """
        addr = arg.split(':')
        if len(addr) == 2:
            try:
                port = int(addr[1])
            except ValueError:
                raise QMPShellBadPort
            return ( addr[0], port )
        # socket path
        return arg

    def _fill_completion(self):
        for cmd in self.cmd('query-commands')['return']:
            self._completer.append(cmd['name'])

    def __completer_setup(self):
        self._completer = QMPCompleter()
        self._fill_completion()
        readline.set_completer(self._completer.complete)
        readline.parse_and_bind("tab: complete")
        # XXX: default delimiters conflict with some command names (eg. query-),
        # clearing everything as it doesn't seem to matter
        readline.set_completer_delims('')

    def __build_cmd(self, cmdline):
        """
        Build a QMP input object from a user provided command-line in the
        following format:
    
            < command-name > [ arg-name1=arg1 ] ... [ arg-nameN=argN ]
        """
        cmdargs = cmdline.split()
        qmpcmd = { 'execute': cmdargs[0], 'arguments': {} }
        for arg in cmdargs[1:]:
            opt = arg.split('=')
            try:
                value = int(opt[1])
            except ValueError:
                value = opt[1]
            qmpcmd['arguments'][opt[0]] = value
        return qmpcmd

    def _execute_cmd(self, cmdline):
        global mtx
        try:
            qmpcmd = self.__build_cmd(cmdline)
        except:
            print 'command format: <command-name> ',
            print '[arg-name1=arg1] ... [arg-nameN=argN]'
            return True
        mtx.acquire()
        resp = self.cmd_obj(qmpcmd)
        mtx.release()
        if resp is None:
            print 'Disconnected'
            return False
        return resp
        return True

    def connect(self):
        self._greeting = qmp.QEMUMonitorProtocol.connect(self)
        self.__completer_setup()

    def show_banner(self, msg='Welcome to the QMP low-level shell!'):
        print msg
        version = self._greeting['QMP']['version']['qemu']
        print 'Connected to QEMU %d.%d.%d\n' % (version['major'],version['minor'],version['micro'])

    def read_exec_command(self, prompt):
        """
        Read and execute a command.

        @return True if execution was ok, return False if disconnected.
        """
        try:
            pnumber = raw_input("Input Parameter Number: ")
            pvalue = raw_input("Input Parameter Value: ")     		
            cmdline = "power-model-param p=\""+pnumber+"\""+" v=\""+pvalue+"\""
            print cmdline
        except EOFError:
            print
            return False
        if cmdline == '':
            for ev in self.get_events():
                print ev
            self.clear_events()
            return True
        else:
            return self._execute_cmd(cmdline)
            
    def no_input_exec_command(self, cmdline):
        """
        Read and execute a command.

        @return True if execution was ok, return False if disconnected.
        """
        if cmdline == '':
            for ev in self.get_events():
                print ev
            self.clear_events()
            return True
        else:
            return self._execute_cmd(cmdline)            

class HMPShell(QMPShell):
    def __init__(self, address):
        QMPShell.__init__(self, address)
        self.__cpu_index = 0

    def __cmd_completion(self):
        for cmd in self.__cmd_passthrough('help')['return'].split('\r\n'):
            if cmd and cmd[0] != '[' and cmd[0] != '\t':
                name = cmd.split()[0] # drop help text
                if name == 'info':
                    continue
                if name.find('|') != -1:
                    # Command in the form 'foobar|f' or 'f|foobar', take the
                    # full name
                    opt = name.split('|')
                    if len(opt[0]) == 1:
                        name = opt[1]
                    else:
                        name = opt[0]
                self._completer.append(name)
                self._completer.append('help ' + name) # help completion

    def __info_completion(self):
        for cmd in self.__cmd_passthrough('info')['return'].split('\r\n'):
            if cmd:
                self._completer.append('info ' + cmd.split()[1])

    def __other_completion(self):
        # special cases
        self._completer.append('help info')

    def _fill_completion(self):
        self.__cmd_completion()
        self.__info_completion()
        self.__other_completion()

    def __cmd_passthrough(self, cmdline, cpu_index = 0):
        return self.cmd_obj({ 'execute': 'human-monitor-command', 'arguments':
                              { 'command-line': cmdline,
                                'cpu-index': cpu_index } })

    def _execute_cmd(self, cmdline):
        if cmdline.split()[0] == "cpu":
            # trap the cpu command, it requires special setting
            try:
                idx = int(cmdline.split()[1])
                if not 'return' in self.__cmd_passthrough('info version', idx):
                    print 'bad CPU index'
                    return True
                self.__cpu_index = idx
            except ValueError:
                print 'cpu command takes an integer argument'
                return True
        resp = self.__cmd_passthrough(cmdline, self.__cpu_index)
        if resp is None:
            print 'Disconnected'
            return False
        assert 'return' in resp or 'error' in resp
        if 'return' in resp:
            # Success
            if len(resp['return']) > 0:
                print resp['return'],
        else:
            # Error
            print '%s: %s' % (resp['error']['class'], resp['error']['desc'])
        return True

    def show_banner(self):
        QMPShell.show_banner(self, msg='Welcome to the HMP shell!')

def die(msg):
    sys.stderr.write('ERROR: %s\n' % msg)
    sys.exit(1)

def fail_cmdline(option=None):
    if option:
        sys.stderr.write('ERROR: bad command-line option \'%s\'\n' % option)
    sys.stderr.write('qemu-shell [ -H ] < UNIX socket path> | < TCP address:port >\n')
    sys.exit(1)

#qemu = []


class CommandPrompt(threading.Thread) :
	
	def __init__(self) :
		#qemu.show_banner()
		super(CommandPrompt, self).__init__()

		self.start()
				

	def run(self) :
		global qemu
		while qemu.read_exec_command('(QEMU) '):
			pass	
		qemu.close()
		
		
class PowerData(threading.Thread) :
	
	def __init__(self) :
		#qemu.show_banner()
		super(PowerData, self).__init__()
		self.start()
		self.prevact = 0
		self.prevslp = 0
		self.datapoints = []
				

	def run(self) :
		global qemu
		global demo
		self.datapoints = [0]
		while True :
			resp=qemu.no_input_exec_command('query-energy')
			#print resp
			activep = resp['return']['1']['value']
			sleepp = resp['return']['2']['value']
			delta = (activep + sleepp) - (self.prevact + self.prevslp)
			averagep =  delta / 1.0
			if len(self.datapoints) > 10 :
				self.datapoints.pop(0)
			self.prevact = activep
			self.prevslp = sleepp
			self.datapoints.append(averagep)
			#print averagep, np.average(self.datapoints, weights=np.exp(range(len(self.datapoints))))  #,  #weights=[1, 2, 10, 25, 50]

			
			#print activep, sleepp, averagep
			pub.sendMessage("average_power", averagep)
			#demo.plt0.newData(activep+sleepp)
			#demo.plt0.updateTimePlot()
			time.sleep(1)
			pass	
		qemu.close()		
	
def varemu_shell():
	global qemu
	global mtx
	mtx = threading.Lock()
	addr = ''
	try:
		if len(sys.argv) == 2:
			qemu = QMPShell(sys.argv[1])
			addr = sys.argv[1]
		elif len(sys.argv) == 3:
			if sys.argv[1] != '-H':
				fail_cmdline(sys.argv[1])
			qemu = HMPShell(sys.argv[2])
			addr = sys.argv[2]
		else:
				fail_cmdline()
	except QMPShellBadPort:
		die('bad port number in command-line')
	try:
		qemu.connect()
	except qmp.QMPConnectError:
		die('Didn\'t get QMP greeting message')
	except qmp.QMPCapabilitiesError:
		die('Could not negotiate capabilities')
	except qemu.error:
		die('Could not connect to %s' % addr)

	
	prompt = CommandPrompt();
	pdata = PowerData();




class PlotDisplay(wx.Panel) :

	def __init__(self, datachannel, parent, size, pos, sampleRate=1, timeRange=(0,120), currentRange=(0,50), camera=None):
		super(PlotDisplay,self).__init__(parent, size=size, pos=pos)
		self.datachannel = datachannel
		(self.dimx,self.dimy) = self.GetSize()
		print pos
		channel = "average_power"
		pub.subscribe(self.newData, channel)
		
		self.imageScale = .95
		(self.dimx,self.dimy) = self.GetSize()
		self.imgx = self.dimx*self.imageScale
		self.imgy = self.dimx*self.imageScale*.75		
		
		self.timeRange = timeRange
		self.currentRange = currentRange
		self.sampleRate = sampleRate
		
		self.nPoints = (timeRange[1] - timeRange[0])*sampleRate+1
		self.xStep = sampleRate/float(timeRange[1] - timeRange[0])
		self.xData = np.linspace(timeRange[0], timeRange[1], self.nPoints).tolist()
		self.yData = np.zeros(self.nPoints).tolist()
		self.camera = camera
		#print self.xData
		self.SetBackgroundColour("white")
		self.newPoints = 0

		
	def newData(self, msg) :
		#new data
		self.yData.pop(0)
		#print msg.data
		try :
			pt = float(msg.data)
		except ValueError :
			pt = 0
			print "Conversion error on channel ", self.datachannel

		self.yData.append(pt)
		self.newPoints = self.newPoints + 1
		#if self.newPoints == 10 :
		wx.CallAfter(self.updateTimePlot)
		self.newPoints = 0
		#print 'new data', self.datachannel
		
	def updateTimePlot(self):
		
		#display
		dc = wx.WindowDC(self)
		#pen = wx.Pen("blue", width = 10, style = wx.SOLID)
		#dc.SetPen(pen)	
		dc.Clear()
		#dc.DrawRectangle(0,0, self.dimx,self.dimy)
		
		subpanel = wx.Panel(self,pos = ((self.dimx-self.imgx)/2, 0), size = (self.imgx,self.imgy) )
		
		sizer = wx.BoxSizer(wx.VERTICAL)
		canvas = plot.PlotCanvas(subpanel)
		canvas.SetFontSizeTitle(point=10)	
		canvas.SetFontSizeAxis(point=10)	
		sizer.Add(canvas, 1, wx.EXPAND, 0)
		subpanel.SetSizer(sizer) 
		subpanel.Layout()
		
		plotData = zip(self.xData,self.yData)
		
		line = plot.PolyLine(plotData, colour='red', width=1)
		
		if self.camera != None :
			title = "Temperature: "+str(self.camera.getTemperature())+" C"
		else :
			title = ""
		
		gc = plot.PlotGraphics([line], title, xLabel='Time (s)',yLabel='Average Power(uW)')
		canvas.Draw(gc, self.timeRange, self.currentRange)

		self.Show()				

			

class Demo(wx.App) :

	def onClose(self,event) :
		self.window.Destroy()
		os._exit(1)

	def __init__(self) :
		super(Demo, self).__init__(False)

		self.hsize = 640
		self.vsize = 480
			
		font = wx.Font(12, wx.FONTFAMILY_MODERN, wx.FONTSTYLE_NORMAL, wx.FONTWEIGHT_NORMAL)			
		self.window = wx.Frame(None, title = "VarEMU Demo", size=(self.hsize,self.vsize), pos=(0,0))
		self.window.SetFont(font)
		self.window.Bind(wx.EVT_CLOSE, self.onClose)

		self.size=(self.hsize,self.vsize)

		self.pPlots = wx.Panel(self.window, size=(self.hsize, self.vsize), pos=(0,0))
		
		self.size = (self.hsize, self.vsize)
		
		self.plt0 = PlotDisplay('0', self.pPlots, self.size, (0,0), currentRange=(0, 500))
		
		self.window.Show(True)
		
		self.plt0.updateTimePlot()




def start_app() :
	global demo
	demo = Demo()
	varemu_shell()
	demo.MainLoop()
	
	

start_app()  
			
