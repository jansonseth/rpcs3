#include "breakpoint_handler.h"

extern void ppu_breakpoint(u32 loc, bool isAdding);

breakpoint_handler::breakpoint_handler() :m_breakpoints_list()
{
	break_on_bpm = false;
}

breakpoint_handler::~breakpoint_handler()
{

}

bool breakpoint_handler::IsBreakOnBPM()
{
	return break_on_bpm;
}

bool breakpoint_handler::HasBreakpoint(u32 loc, bs_t<breakpoint_type> type, u8 size)
{
	auto start = m_breakpoints_list.lower_bound(loc);
	auto end = m_breakpoints_list.upper_bound(loc + size - 1);

	return (start != end) && ((m_breakpoints_list[loc] & type) == type);
}

bool breakpoint_handler::AddBreakpoint(u32 loc, bs_t<breakpoint_type> type)
{
	m_breakpoints_list[loc] = type;

	if (type & breakpoint_type::bp_execute)
	{
		ppu_breakpoint(loc, true);
	}
	return true;
}

bool breakpoint_handler::RemoveBreakpoint(u32 loc)
{
	auto it = m_breakpoints_list.find(loc);
	if (it == m_breakpoints_list.end())
	{
		return false;
	}

	m_breakpoints_list.erase(it);

	if (it->second & breakpoint_type::bp_execute)
	{
		ppu_breakpoint(loc, false);
	}

	return true;
}
