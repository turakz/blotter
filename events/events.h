#ifndef _EVENTS_H
#define _EVENTS_H

#include <unordered_map>
namespace blotter {
    namespace events {
        class Event {
        public:
            virtual ~Event(){};
            virtual void Handle() = 0;
        };
        class EventManager {
        public:
            virtual ~EventManager(){};
            virtual void add_event(const std::string& which, Event* event) = 0;
            virtual void remove_event(const std::string& which, Event* event) = 0;
            virtual bool raise_event(const std::string& which) = 0;
        };
        class event_manager : public EventManager {
        public:
            ~event_manager();
            void add_event(const std::string& which, Event* event);
            void remove_event(const std::string& which, Event* event);
            bool raise_event(const std::string& event);
        private:
            std::unordered_map<std::string, Event*> events_;
        };
    }//events
}//blotter
#endif
