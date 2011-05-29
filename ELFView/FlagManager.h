#ifndef FLAG_MANAGER_H
#define FLAG_MANAGER_H

#include <wx/string.h>

#include <vector>

class FlagManager
{
public:
	struct Value {
		wxString name;
		int value;

		Value(wxString n, int v) : name(n), value(v) {}
	};

	enum Set {
		SetElfType,
		SetSectionType,
		SetSegmentType,
		SetSymbolBind,
		SetSymbolType,
		SetDynamicType,
	};

	static std::vector<Value> GetValues(Set set);
	static wxString GetDescription(Set set, int value);
};

#endif