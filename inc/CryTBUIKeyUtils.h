#pragma once
#include <tb_widgets.h>
#include <IInput.h>

namespace TurboBadgerUIPlugin
{
	enum EUnicodeSpecialKeys
	{
		eUSK_Backspace = 8,
		eUSK_Tab = 9,
		eUSK_Enter = 13,
		eUSK_Escape = 27,
	};

	class CCryTBUIKeyUtils
	{
	public:

		/// <summary>
		/// If possivle, converts a CE unicode event to a TB special key
		/// </summary>
		/// <param name="inputChar">The input character of the SUnicodeEvent.</param>
		/// <returns>The converted key or TB_KEY_UNDEFINED if it can't be converted</returns>
		static tb::SPECIAL_KEY CryUniToTBSpecial(uint32 inputChar)
		{
			// SPECIAL KEYS
			tb::SPECIAL_KEY specialKey = tb::TB_KEY_UNDEFINED;

			// tb::SPECIALKEY elements that are not handeled here do not generate unicode events and
			// are handeled inside HandleKeyboardEvents
			switch (inputChar)
			{
			case(eUSK_Backspace) :
				specialKey = tb::TB_KEY_BACKSPACE;
				break;
			case(eUSK_Tab) :
				specialKey = tb::TB_KEY_TAB;
				break;
			case (eUSK_Enter) :
				specialKey = tb::TB_KEY_ENTER;
				break;
			case (eUSK_Escape) :
				specialKey = tb::TB_KEY_ESC;
			default:
				break;
			}

			return specialKey;
		}

		/// <summary>
		/// If possible converts a CE key to a tb::SPECIAL_KEY
		/// </summary>
		/// <param name="cryKey">The CE key.</param>
		/// <returns>A tb::SPECIAL_KEY resembling the CE key or tb::TB_KEY_UNDEFINED of no conversion possible</returns>
		static tb::SPECIAL_KEY CryKeyToTBSpecial(EKeyId cryKey)
		{
			tb::SPECIAL_KEY tbKey = tb::TB_KEY_UNDEFINED;

			switch (cryKey)
			{
			case (eKI_Up) :
				tbKey = tb::TB_KEY_UP;
				break;
			case (eKI_Down) :
				tbKey = tb::TB_KEY_DOWN;
				break;
			case (eKI_Left) :
				tbKey = tb::TB_KEY_LEFT;
				break;
			case (eKI_Right) :
				tbKey = tb::TB_KEY_RIGHT;
				break;
			case (eKI_PgUp) :
				tbKey = tb::TB_KEY_PAGE_UP;
				break;
			case (eKI_PgDn) :
				tbKey = tb::TB_KEY_PAGE_DOWN;
				break;
			case (eKI_Home) :
				tbKey = tb::TB_KEY_HOME;
				break;
			case (eKI_End) :
				tbKey = tb::TB_KEY_END;
				break;
			case (eKI_Insert) :
				tbKey = tb::TB_KEY_INSERT;
				break;
			case (eKI_Delete) :
				tbKey = tb::TB_KEY_DELETE;
				break;
			case (eKI_F1) :
			case (eKI_F2) :
			case (eKI_F3) :
			case (eKI_F4) :
			case (eKI_F5) :
			case (eKI_F6) :
			case (eKI_F7) :
			case (eKI_F8) :
			case (eKI_F9) :
			case (eKI_F10) :
				tbKey = (tb::SPECIAL_KEY)((int)cryKey - _cryF1F10Offset);
				break;
			case (eKI_F11) :
			case (eKI_F12) :
				tbKey = (tb::SPECIAL_KEY)((int)cryKey - _cryf11f12Offset);
			default:
				break;
			}
		}

		/// <summary>
		/// Converts a CE modifier mask to a TB modifier mask
		/// </summary>
		/// <param name="cryModifier">The CE modifier mask.</param>
		/// <returns>A TB modifier mask resembling the given CE modifier mask</returns>
		static tb::MODIFIER_KEYS CryModiToTBModi(EModifierMask cryModifier)
		{
			// MODIFIERS
			tb::MODIFIER_KEYS modifierKeys = tb::TB_MODIFIER_NONE;

			if (cryModifier & eMM_Ctrl)
			{
				modifierKeys |= tb::TB_CTRL;
			}

			if (cryModifier & eMM_Alt)
			{
				modifierKeys |= tb::TB_ALT;
			}

			if (cryModifier & eMM_Shift)
			{
				modifierKeys |= tb::TB_SHIFT;
			}

			if (cryModifier & eMM_Win)
			{
				modifierKeys |= tb::TB_SUPER;
			}

			return modifierKeys;
		}

	private:

		/// <summary>
		/// Offset between eKI_F1...eKIF10 and TB_KEY_F1...TB_KEY_F10
		/// </summary>
		static const int _cryF1F10Offset = 43;
		/// <summary>
		/// Offset between eKI_F11...eKIF12 and TB_KEY_F11...TB_KEY_F12
		/// </summary>
		static const int _cryf11f12Offset = 57;
	};
}