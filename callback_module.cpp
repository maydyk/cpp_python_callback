#include <memory>
#include <iostream>

// Simulate our library
namespace library {
    // Callback pure interface
    class Callback{
    public:
        virtual ~Callback() {
            // Just for test!
            std::cout << "~Callback() destructed!" << std::endl;
        } 
        virtual void onProgress(int progress) = 0;
    };

    // Some library class who gets and stores callback instance
    class Library {
        std::shared_ptr<Callback> callback;
    public:
        Library(std::shared_ptr<Callback> _callback)
        : callback { _callback } {
            std::cout << "Library() constructed!" << std::endl;
        }

        ~Library() {
            // Ligrary is free!
            std::cout << "~Library() destructed!" << std::endl;
        }

        void func() {
            // Use callback
            if (callback != nullptr) {
                callback->onProgress(66);
            }
        }
    };

    // Show module is unloaded.
    struct Lifecycle {
        ~Lifecycle() {
            std::cout << "Lifecycle has terminated." << std::endl;
        }
    };

    Lifecycle lc;
}

#include <boost/python.hpp>

namespace py = boost::python;

namespace {
    // Python wrapper for callback
    struct CallbackWrapper
        : library::Callback
        , py::wrapper<library::Callback>
        {
            virtual void onProgress(int progress) override {
                this->get_override("onProgress")(progress);
            }
        };

    // Special testing callback
    struct CallbackWrapperTest
        : library::Callback
        , py::wrapper<library::Callback>
        {
            virtual void onProgress(int progress) override {
                std::cout << "TEST: onProgress(" << progress << ")" << std::endl;
            }
        };

    // Creation finctuon
    std::shared_ptr<library::Library> createLibrary(std::shared_ptr<library::Callback> callback) {
        return std::make_shared<library::Library>(callback);
    }
}


BOOST_PYTHON_MODULE(callback) {
    py::class_<library::Library, boost::noncopyable>(
        "Library",
        py::no_init
        )
        .def("__init__", py::make_constructor(&createLibrary))
        .def("func", &library::Library::func)
        ;
    
    py::class_<CallbackWrapper, boost::noncopyable>("Callback");

    py::class_<CallbackWrapperTest, boost::noncopyable>("CallbackTest");
}
