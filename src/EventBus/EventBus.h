#ifndef EVENTBUS_H
#define EVENTBUS_H

#include <map>
#include <typeindex>
#include <list>
#include "../Logger/Logger.h"

class Event {
public:
    Event() = default;
};

class IEventCallBack {
private:
    virtual void Call(Event &e) = 0;

public:
    virtual  ~IEventCallBack() = default;

    void Execute(Event &e) {
        Call(e);
    }
};

template<typename TOwner, typename TEvent>
class EventCallBack : public IEventCallBack {
private:
    typedef void (TOwner::*CallBackFunction)(TEvent &);

    TOwner *ownerInstance;

    CallBackFunction callBackFunction;

    virtual void Call(Event &e) override {
        std::invoke(callBackFunction, ownerInstance, static_cast<TEvent &>(e));
    }

public:
    EventCallBack(TOwner *ownerInstance, CallBackFunction callBackFunction) {
        this->ownerInstance = ownerInstance;
        this->callBackFunction = callBackFunction;
    }

    virtual ~EventCallBack() override = default;
};

typedef std::list<std::unique_ptr<IEventCallBack>> HandlerList;

class EventBus {
private:
    std::map<std::type_index, std::unique_ptr<HandlerList>> subscribers;
public:
    EventBus() {
        Logger::Log("EventBus constructor called!");
    }

    ~EventBus() {
        Logger::Log("EventBus destructor called!");
    }

    template<typename TEvent, typename TOwner>
    void SubscribeToEvent(TOwner *ownerInstance, void (TOwner::*callBackFunction)(TEvent &)) {
        auto subscriber = std::make_unique<EventCallBack<TOwner, TEvent>>(ownerInstance, callBackFunction);
        subscribers[typeid(TEvent)]->push_back(std::move(subscriber));
    };

    // void EmitEvent<>
};

#endif //EVENTBUS_H
