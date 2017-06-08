/*
 * Copyright (C) 2015-2017 Cybernetica
 *
 * Research/Commercial License Usage
 * Licensees holding a valid Research License or Commercial License
 * for the Software may use this file according to the written
 * agreement between you and Cybernetica.
 *
 * GNU General Public License Usage
 * Alternatively, this file may be used under the terms of the GNU
 * General Public License version 3.0 as published by the Free Software
 * Foundation and appearing in the file LICENSE.GPL included in the
 * packaging of this file.  Please review the following information to
 * ensure the GNU General Public License version 3.0 requirements will be
 * met: http://www.gnu.org/copyleft/gpl-3.0.html.
 *
 * For further information, please contact us at sharemind@cyber.ee.
 */

#include "TdbConfiguration.h"

#include <sharemind/Configuration.h>
#include <type_traits>


namespace sharemind {

TdbConfiguration::TdbConfiguration(std::string const & filename) {
    Configuration config(filename);
    for (auto const & v : config) {
        std::string const section(v.key());
        if (section.find("DBModule") == 0u) {
            m_dbModuleList.emplace_back(
                    DbModuleEntry{
                        v.get<std::string>("File"),
                        v.get<std::string>("Configuration", "")});
        } else if (section.find("DataSource") == 0u) {
            m_dataSourceList.emplace_back(
                    DataSourceEntry{
                        v.get<std::string>("Name"),
                        v.get<std::string>("DBModule"),
                        v.get<std::string>("Configuration")});
        }
    }
}

} /* namespace sharemind { */
