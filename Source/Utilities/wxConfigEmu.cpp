/*
 * This file is part of pcsp.
 *
 * pcsp is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * pcsp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with pcsp.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "wxConfigEmu.h"


#include "Common.h"
#include "wx/config.h"
//#include "wx/confbase.h" 
#include "wx/fileconf.h" 
//#include "wx/msw/regconf.h"

#undef wxUSE_CONFIG_NATIVE 
#define wxUSE_CONFIG_NATIVE  0
bool wxConfigEmu::load()
{
  wxFileConfig *pConfig = new wxFileConfig("", "", "pcsp.ini","", wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_RELATIVE_PATH);
  wxConfigBase::Set(pConfig);

	return true;
}
bool wxConfigEmu::save()
{
   return true;
}
void wxConfigEmu::set(const string &sect, const string &name, u32 value)
{
  wxConfigBase *pConfig = wxConfigBase::Get();
  pConfig->SetPath(wxT("/" + sect + "/"));
  pConfig->Write(wxT(name), (long)value);
  pConfig->SetPath(wxT("/"));
}
bool wxConfigEmu::get(const string &sect, const string &name, u32* value, u32 def)
{
   wxConfigBase *pConfig = wxConfigBase::Get();
   pConfig->SetPath(wxT("/" + sect + "/"));
   if(!pConfig->Read(wxT(name),(long*)value,(long)def))
   {
   
    pConfig->SetPath(wxT("/"));
    return false;
   }
    pConfig->SetPath(wxT("/"));
    return true;
}
bool wxConfigEmu::get(const string &sect, const string &name, bool *value, bool def)
{
   wxConfigBase *pConfig = wxConfigBase::Get();
   pConfig->SetPath(wxT("/" + sect + "/"));
   if(!pConfig->Read(wxT(name),value,def))
   {
   
    pConfig->SetPath(wxT("/"));
    return false;
   }
    pConfig->SetPath(wxT("/"));
    return true;
}
