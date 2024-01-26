#include <boost/python.hpp>

#include <memory>
#include <iostream>

namespace py = boost::python;

namespace library {
    class Callback{
    public:
        virtual ~Callback() {
            // Just for test!
            std::cout << "~Callback() destructed!" << std::endl;
        } 
        virtual void onProgress(int progress) = 0;
    };

    class Library {
        std::shared_ptr<Callback> callback;
    public:
        Library(std::shared_ptr<Callback> _callback)
        : callback { _callback } {
            std::cout << "Library() constructed!" << std::endl;
        }

        ~Library() {
            std::cout << "~Library() destructed!" << std::endl;
        }

        void func() {
            if (callback != nullptr) {
                callback->onProgress(66);
            }
        }
    };

    struct Lifecycle {
        ~Lifecycle() {
            std::cout << "Lifecycle has terminated." << std::endl;
        }
    };

    Lifecycle lc;
}

namespace {
    struct CallbackWrapper
        : library::Callback
        , py::wrapper<library::Callback>
        {
            virtual void onProgress(int progress) override {
                this->get_override("onProgress")(progress);
            }
        };

    struct CallbackWrapperTest
        : library::Callback
        , py::wrapper<library::Callback>
        {
            virtual void onProgress(int progress) override {
                std::cout << "TEST: onProgress(" << progress << ")" << std::endl;
            }
        };
}

    std::shared_ptr<library::Library> createLibrary(std::shared_ptr<library::Callback> callback) {
        // std::shared_ptr<CallbackWrapper> cb = std::shared_ptr<CallbackWrapper>(&callback, [](auto*) {} );
        return std::make_shared<library::Library>(callback);
    }


BOOST_PYTHON_MODULE(callback) {
    py::class_<library::Library, boost::noncopyable>(
        "Library",
        py::no_init
        // py::init<CallbackWrapper*>()[py::with_custodian_and_ward<1,2>()]
        )
        .def("__init__", py::make_constructor(&createLibrary))
        .def("func", &library::Library::func)
        ;
    
    py::class_<CallbackWrapper, boost::noncopyable>("Callback");
    py::class_<CallbackWrapperTest, boost::noncopyable>("CallbackTest");
}