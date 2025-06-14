#pragma once

#include <string>

#include "Vroom/Api.h"
#include "Vroom/Event/Codes.h"
#include "Vroom/Event/Trigger/TriggerCallback.h"
#include "Vroom/Event/Event.h"

namespace vrm
{

class TriggerManager;

/**
 * @brief Trigger binder class.
 * 
 * This class makes it easier to bind inputs and callbacks to a trigger.
 * 
 */
class VRM_API TriggerBinder
{
public:

    /**
     * @brief Construct a new Trigger Binder object
     * 
     * @param tm TriggerManager to bind the trigger to.
     * @param triggerName Name of the trigger.
     */
    TriggerBinder(TriggerManager& tm, const std::string& triggerName) : m_Tm(tm), m_TriggerName(triggerName) {}

    TriggerBinder(const TriggerBinder& other) = delete;

    /**
     * @brief Construct a new Trigger Binder object
     * 
     * @param other TriggerBinder to move.
     */
    TriggerBinder(TriggerBinder&& other);

    virtual ~TriggerBinder();

    /**
     * @brief Bind a key input to the trigger.
     * 
     * @param key Key to bind.
     * @return TriggerBinder A binder to the same Trigger instance.
     */
    TriggerBinder& bindInput(const KeyCode& key);

    /**
     * @brief Bind a mouse input to the trigger.
     * 
     * @param mouse Mouse button to bind.
     * @return TriggerBinder A binder to the same Trigger instance.
     */
    TriggerBinder& bindInput(const MouseCode& mouse);

    /**
     * @brief Unbind a key input from the trigger.
     * 
     * @param key Key to unbind.
     * @return TriggerBinder A binder to the same Trigger instance.
     */
    TriggerBinder& unbindInput(const KeyCode& key);

    /**
     * @brief Unbind a mouse input from the trigger.
     * 
     * @param mouse Mouse button to unbind.
     * @return TriggerBinder A binder to the same Trigger instance.
     */
    TriggerBinder& unbindInput(const MouseCode& mouse);

    /**
     * @brief Bind a callback to the trigger.
     * 
     * @param callback Callback to bind.
     * @return TriggerBinder A binder to the same Trigger instance.
     */
    TriggerBinder& bindCallback(const TriggerCallback& callback);

private:
    TriggerManager& m_Tm;
    std::string m_TriggerName;
};

}