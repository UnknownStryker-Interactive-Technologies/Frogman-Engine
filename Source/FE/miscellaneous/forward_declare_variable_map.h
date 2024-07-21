#ifndef _FE_FORWARD_DECLARE_VARIABLE_MAP_H_
#define _FE_FORWARD_DECLARE_VARIABLE_MAP_H_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.

namespace FE::framework
{
    class property;
}

#define ENABLE_FE_CLASS_SERIALIZATION() friend class ::FE::framework::property;

#endif