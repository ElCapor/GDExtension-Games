#ifndef SIGNAL_SYSTEM_H
#define SIGNAL_SYSTEM_H
/*
CUSTOM SIGNAL SYSTEM FOR GODOT TO MAKE MY LIFE EASIER
*/
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/classes/node.hpp>

typedef void (*SignalCallback)(const godot::Variant**, int, godot::Variant&, GDExtensionCallError&);
typedef void (*ClassSignalCallback)(godot::Object*, const godot::Variant**, int, godot::Variant&, GDExtensionCallError&);


class SignalCallable : public godot::CallableCustom
{
    virtual uint32_t hash() const
    {
        return godot::VariantHasher::hash(this->get_as_text());
    }

    virtual godot::String get_as_text() const
    {
        return "<SignalCallable>";
    }

    static bool compare_equal_func( const CallableCustom *inA, const CallableCustom *inB )
    {
        return inA == inB;
    }

    virtual CompareEqualFunc get_compare_equal_func() const
    {
        return &SignalCallable::compare_equal_func;
    }

    static bool compare_less_func( const CallableCustom *inA, const CallableCustom *inB )
    {
        return reinterpret_cast<const void *>( inA ) < reinterpret_cast<const void *>( inB );
    }

    virtual CompareLessFunc get_compare_less_func() const
    {
        return &SignalCallable::compare_less_func;
    }

    bool is_valid() const
    {
        return true;
    }

    virtual godot::ObjectID get_object() const
    {
        return godot::ObjectID();
    }
};

#define SIGNAL_CALLABLE_NAME(signal_name) \
    virtual godot::String get_as_text() const \
    { \
        return "<"#signal_name">"; \
    } \

/*Connect a simple function (can be lambda)*/
class CallbackSignalConnector {
public:

    typedef void (*SignalCallback)(const godot::Variant**, int, godot::Variant&, GDExtensionCallError&);

    CallbackSignalConnector() {};

    void connect(godot::Node* node, const godot::String& signal, SignalCallback callback) {
        node->connect(signal, make_callable(callback));
    };

private:
    godot::CallableCustom* make_callable(SignalCallback callback) {
        class FunctionPointerCallable : public SignalCallable {
        private:
            SignalCallback m_callback;
            
        public:
            FunctionPointerCallable(SignalCallback callback) 
                : m_callback(callback) {}

            virtual void call(const godot::Variant** inArguments, int inArgcount,
                              godot::Variant& outReturnValue, GDExtensionCallError& outCallError) const override {
                (m_callback)(inArguments, inArgcount, outReturnValue, outCallError);
            }
        };

        return memnew(FunctionPointerCallable(callback));
    }
};


/*
Connect to a signal from a member function of a class
*/
template<typename T>
class SignalConnector {
public:

    typedef void (T::*SignalCallback)(const godot::Variant**, int, godot::Variant&, GDExtensionCallError&);

    SignalConnector(T* object) : m_object(object) {}

    void connect(godot::Node* node, const godot::String& signal, SignalCallback callback) {
        node->connect(signal, make_callable(callback));
    };

private:
    T* m_object;

    godot::CallableCustom* make_callable(SignalCallback callback) {
        class FunctionPointerCallable : public SignalCallable {
        private:
            T* m_object;
            SignalCallback m_callback;
            
        public:
            FunctionPointerCallable(T* object, SignalCallback callback) 
                : m_object(object), m_callback(callback) {}

            virtual void call(const godot::Variant** inArguments, int inArgcount,
                              godot::Variant& outReturnValue, GDExtensionCallError& outCallError) const override {
                (m_object->*m_callback)(inArguments, inArgcount, outReturnValue, outCallError);
            }
        };

        return memnew(FunctionPointerCallable(m_object, callback));
    }
};


#endif