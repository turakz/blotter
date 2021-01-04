#ifndef _EVENTS_H
#define _EVENTS_H

#include <boost/system/error_code.hpp>

#include <iostream>
#include <unordered_map>

namespace blotter {
    namespace events {
        class event_base {
        public:
            event_base() = default;
            event_base(const event_base& eb) = delete;
            event_base& operator=(const event_base& eb) = delete;
            event_base(event_base&& eb) = delete;
            event_base& operator=(event_base&& eb) = delete;
            // events need to be deleted by an event manager (which owns the event/resource)
            // let event manager delete derived event types but there's no need to actually implement
            virtual ~event_base() = default;
            // polymorphic handling
            virtual void handler(boost::system::error_code& ec) = 0;
        };
        class event_manager_base {
        public:
            event_manager_base() = default;
            event_manager_base(const event_manager_base& emb) = delete;
            event_manager_base& operator=(const event_manager_base& emb) = delete;
            event_manager_base(event_manager_base&& emb) = delete;
            event_manager_base& operator=(event_manager_base&& emb) = delete;
            // event managers own resources (registered events) -> event manager implements its own destructor
            virtual ~event_manager_base(){};
            virtual void add_event(const std::string& which, event_base* event) = 0;
            virtual void remove_event(const std::string& which) = 0;
            // this interface is meant to be used in conjunction with boost facilities -- give event handlers access to boost error state
            virtual bool raise_event(const std::string& which, boost::system::error_code& ec) = 0;
        };
    }//events
}//blotter
#endif
