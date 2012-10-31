from qmp import QEMUMonitorProtocol

def main(argv=None):
	x = QEMUMonitorProtocol("qmp-sock_01", False)
	print x.connect()
	print x.cmd("power-model-param", {"p":"0", "v":"10"})

if __name__ == "__main__":
	main()
