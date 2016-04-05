/* TurboBadgerUI_Plugin - for licensing and copyright see license.txt */

#include <IPluginBase.h>

#pragma once

/**
* @brief TurboBadgerUI Plugin Namespace
*/
namespace TurboBadgerUIPlugin
{
    /**
    * @brief plugin TurboBadgerUI concrete interface
    */
    struct IPluginTurboBadgerUI
    {
        /**
        * @brief Get Plugin base interface
        */
        virtual PluginManager::IPluginBase* GetBase() = 0;

        // TODO: Add your concrete interface declaration here
    };
};