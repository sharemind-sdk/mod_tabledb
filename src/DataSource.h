/*
 * Copyright (C) 2015 Cybernetica
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

#ifndef SHAREMIND_MOD_TABLEDB_DATASOURCE_H
#define SHAREMIND_MOD_TABLEDB_DATASOURCE_H

#include <sharemind/dbcommon/datasourceapi.h>
#include <string>


namespace sharemind  {

class __attribute__ ((visibility("internal"))) DataSource
    : private ::SharemindDataSource
{

private: /* Types: */

    using Wrapper = ::SharemindDataSource;

public: /* Methods: */

    DataSource(std::string name, std::string module, std::string conf);

    inline std::string & name() { return m_name; }
    inline const std::string & name() const { return m_name; }

    inline std::string & module() { return m_module; }
    inline const std::string & module() const { return m_module; }

    inline std::string & conf() { return m_conf; }
    inline const std::string & conf() const { return m_conf; }

    static DataSource & fromWrapper(Wrapper & wrapper) noexcept
    { return static_cast<DataSource &>(wrapper); }

    inline Wrapper * getWrapper() { return this; }
    inline Wrapper const * getWrapper() const { return this; }

private: /* Fields: */

    std::string m_name;
    std::string m_module;
    std::string m_conf;

}; /* class DataSource { */

} /* namespace sharemind { */

#endif /* SHAREMIND_MOD_TABLEDB_DATASOURCE_H */
