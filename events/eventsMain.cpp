// expose event handling interfaces
#include "events.h"

#include <iostream>
#include <cassert>
// extend the interface as needed -> ie: TCP socket events
// resolve event
// -------------
class on_resolve : public blotter::events::Event {
public:
    on_resolve()
        : event_name_{"on resolve"} {}
    void Handle()
    {
        std::cout << "event raised, handling: " << event_name_ << std::endl;
    }
private:
    std::string event_name_;
};
// handshake event
// ---------------
class on_handshake : public blotter::events::Event {
public:
    on_handshake()
        : event_name_{"on handshake"} {}
    void Handle()
    {
        std::cout << "event raised, handling: " << event_name_ << std::endl;
    }
private:
    std::string event_name_;
};
// connect event
// -------------
class on_connect : public blotter::events::Event {
public:
    on_connect()
        : event_name_{"on connect"} {}
    void Handle()
    {
        std::cout << "event raised, handling: " << event_name_ << std::endl;
    }
private:
    std::string event_name_;
};
// request event (send)
// --------------------
class on_request : public blotter::events::Event {
public:
    on_request()
        : event_name_{"on request"} {}
    void Handle()
    {
        std::cout << "event raised, handling: " << event_name_ << std::endl;
    }
private:
    std::string event_name_;
};
// response event (received)
// -------------------------
class on_response : public blotter::events::Event {
public:
    on_response()
        : event_name_{"on response"} {}
    void Handle()
    {
        std::cout << "event raised, handling: " << event_name_ << std::endl;
    }
private:
    std::string event_name_;
};
// disconnect event
// ----------------
class on_disconnect : public blotter::events::Event {
public:
    on_disconnect()
        : event_name_{"on disconnect"} {}
    void Handle()
    {
        std::cout << "event raised, handling: " << event_name_ << std::endl;
    }
private:
    std::string event_name_;
};
// close event
// -----------
class on_close : public blotter::events::Event {
public:
    on_close()
        : event_name_{"on close"} {}
    void Handle()
    {
        std::cout << "event raised, handling: " << event_name_ << std::endl;
    }
private:
    std::string event_name_;
};


int main(void)
{
    std::cout << "example for implementing an event system using blotter::events" << std::endl;
    std::cout << "constructing blotter::events::event_manager..." << std::endl;

    // events.h includes an interface for managing registered events
    blotter::events::event_manager em {};
    std::cout << "adding use-case specific concrete events..." << std::endl;
    std::cout << "event: on resolve" << std::endl;

    em.add_event("on resolve", new on_resolve {}); //note: this is just an example -> possible leak here if program terminates unexpectedly and ::event_manager destructor doesn't get called
    std::cout << "event: on handshake" << std::endl;
    em.add_event("on handshake", new on_handshake {});
    std::cout << "event: on connect" << std::endl;
    em.add_event("on connect", new on_connect {});
    std::cout << "event: on request" << std::endl;
    em.add_event("on request", new on_request {});
    std::cout << "event: on response" << std::endl;
    em.add_event("on response", new on_response {});
    std::cout << "event: on disconnect" << std::endl;
    em.add_event("on disconnect", new on_disconnect {});
    std::cout << "event: on close" << std::endl;
    em.add_event("on close", new on_close {});

    std::cout << "raising events in no particular order to demonstrate firing events as needed..." << std::endl;
    auto connectEvent = em.raise_event("on connect");
    auto resolveEvent = em.raise_event("on resolve");
    auto closeEvent = em.raise_event("on close");
    auto disconnectEvent = em.raise_event("on disconnect");
    auto requestEvent = em.raise_event("on request");
    auto responseEvent = em.raise_event("on response");
    auto handshakeEvent = em.raise_event("on handshake");
    
    // raise_event returns true when an event was fired (ie, an event that can be fired exists)
    // demonstrate we raised the ones we wanted to
    std::cout << "checking to make sure all registered events fired..." << std::endl;
    assert
    (
        connectEvent
        && resolveEvent
        && closeEvent
        && disconnectEvent
        && requestEvent
        && responseEvent
        && handshakeEvent
    ); 
    std::cout << "all registered events fired!" << std::endl;
    // the system will not raise unregistered events
    std::cout << "firing an unregistered event..." << std::endl;
    auto unregisteredEvent = em.raise_event("unregistered event");
    assert(unregisteredEvent == false);
    if (!unregisteredEvent)
    {
        std::cout << "unregistered event was not fired" << std::endl;
    }
    std::cout << "events-handling done!" << std::endl;
    // event_manager destructor calls delete on every Event object registered to it
    std::cout << "destroying manager and registered events for the context..." << std::endl;
    return 0;
}
