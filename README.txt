Developer notes:
===============

To gernate solution files on windows:
1. checkout a copy of gyp to v8\build\gyp
http://gyp.googlecode.com/svn/trunk
2. checkout a copy of python windows (cgywin python does not work) to v8\third_party\python_26
http://src.chromium.org/svn/trunk/tools/third_party/python_26
3. open cmd or cgywin, run build.bat

To compile on windows
1. Need a copy of python windows as mention easlier
2. need to check out another copy of cygwin to v8\third_party\cygwin to run the build
http://src.chromium.org/svn/trunk/deps/third_party
3. open xruby.sln with visual c++ express 2010 to compile