#include "../v8/src/v8.h"
using namespace v8;
//#include "ruby_parser.h"

int main(int argc, char* argv[]) {

  // Create a stack-allocated handle scope.
  HandleScope handle_scope;

  // Create a new context.
  Persistent<Context> context = Context::New();//will also initilize v8 if not done yet
  
  // Enter the created context for compiling and
  // running the hello world script. 
  Context::Scope context_scope(context);

  // Create a string containing the JavaScript source code.
  Handle<String> source = String::New("'Hello' + ', World!'");

  // Compile the source code.
  Handle<Script> script = Script::Compile(source);//api.cc

  // Run the script to get the result.
  Handle<Value> result = script->Run();

  // Dispose the persistent context.
  context.Dispose();

  // Convert the result to an ASCII string and print it.
  String::AsciiValue ascii(result);
  printf("%s\n", *ascii);
  return 0;
}