#include "events.h"

#include <iostream>
#include <unordered_map>

blotter::events::event_manager::~event_manager()
{
    if (events_.size() > 0)
    {
        for (std::unordered_map<std::string, event_base*>::iterator itr = std::begin(events_); 
                itr != std::end(events_); ++itr)
        {
            delete itr->second;
        }
    }
}

void blotter::events::event_manager::add_event(const std::string& which, event_base* event)
{
    auto e = events_.find(which);
    if (e == std::end(events_))
    {
        events_.insert(std::make_pair(which, event));
    }
    // notify observers we've added an event
}
void blotter::events::event_manager::remove_event(const std::string& which, event_base* event)
{
    auto e = events_.find(which);
    if (e != std::end(events_))
    {
        events_.erase(which);
    }
    // notify observers we've removed an event
}
bool blotter::events::event_manager::raise_event(const std::string& which)
{
    auto e = events_.find(which);
    if (e != std::end(events_))
    {
        events_[which]->Handle();
        return true;
    }
    return false;
}
