#include "TestPlugin.h"

ENDSTONE_PLUGIN("test_plugin", "0.1.0", TestPlugin)
{
    description = "Test Plugin";

    command("test");
}