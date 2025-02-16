#!python3

import callback

class PyCallback(callback.Callback):
    '''Python callback implementation'''
    def onProgress(self, progress):
        print("PY onProgress", progress)

cb = PyCallback()

# Replacing by this works fine but useless
# cb = callback.CallbackTest()
lib = callback.Library(cb)

lib.func()

# explicit library deletion works fine  
# del lib
print("==========================")
