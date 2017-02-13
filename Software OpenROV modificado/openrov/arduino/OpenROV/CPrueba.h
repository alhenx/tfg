#pragma once

// Includes
#include "CModule.h"

class CPrueba : public CModule
{
public:
    void Initialize();
    void Update( CCommand& commandIn );
};

