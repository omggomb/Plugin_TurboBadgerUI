/* TurboBadgerUI_Plugin - for licensing and copyright see license.txt */

#pragma once

#define eCryModule eCryM_Game
// Insert your headers here
#include <platform.h>
#include <algorithm>
#include <vector>
#include <memory>
#include <list>
#include <functional>
#include <limits>
#include <smartptr.h>
#include <CryThread.h>
#include <Cry_Math.h>
#include <ISystem.h>
#include <I3DEngine.h>
#include <IInput.h>
#include <IConsole.h>
#include <ITimer.h>
#include <ILog.h>
#include <IGameplayRecorder.h>
#include <ISerialize.h>


#include <CryWindows.h>

#ifndef _FORCEDLL
#define _FORCEDLL
#endif

#ifndef TURBOBADGERUIPLUGIN_EXPORTS
#define TURBOBADGERUIPLUGIN_EXPORTS
#endif

#pragma warning(disable: 4018)  // conditional expression is constant

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
