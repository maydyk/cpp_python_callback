import sys
sys.path.append(r"./build")
import callback

class PyCallback(callback.Callback):
    def onProgress(self, progress):
        print("PY onProgress", progress, "\n")

# cb = PyCallback()
cb = callback.CallbackTest()

lib = callback.Library(cb)

lib.func()

# del lib
print("==========================")