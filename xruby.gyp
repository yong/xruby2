{
  'includes': ['v8/build/common.gypi'],
  'target_defaults': {
    'type': 'executable',
    'dependencies': [
      'v8/tools/gyp/v8.gyp:v8',
    ],
    'include_dirs': [
      'v8/include',
      'v8/src',
    ],
  },
  'targets': [
    {
      'target_name': 'xruby',
      'sources': [
        'src/main.cpp',
      ],
      'include_dirs': [
        #'ruby19/include',
      ],
    }
  ],
}
