//TODO (gdrane)
// battery_model_init () 
// {
// I. This is just if you need it. If your load does not change with time, from graph
// that Carson Umsted sent me for a constant load you don't need any of this 
// But if your load changes with time then you need this stuff written below 
// 
//  1. Create a new Bottom Half
//  2. Define a function that will be called when the bottom half is executed
// 	 the callback will contain your actual battery_model
//  3. Initialize ptimer with the bottom - half
//  4. Set the period of the timer to a time when the battery characteristic change
// II. You will definitely need elasped time. 
// }
//
// void battery_model(void* opaque)
// {
//  // Logic of the battery model
// }
