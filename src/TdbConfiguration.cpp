 /*
 * This file is a part of the Sharemind framework.
 * Copyright (C) Cybernetica AS
 *
 * All rights are reserved. Reproduction in whole or part is prohibited
 * without the written consent of the copyright owner. The usage of this
 * code is subject to the appropriate license agreement.
 */

#include "TdbConfiguration.h"

#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>


namespace pt = boost::property_tree;

namespace sharemind {

bool TdbConfiguration::load(const std::string & filename) {

    // Define the configuration property tree:
    pt::ptree config;
    DbModuleList newDbModuleList;
    DataSourceList newDataSourceList;

    // Parse the configuration file into the property tree:
    try {
        pt::read_ini(filename, config);
        for (const pt::ptree::value_type & v : config) {
            std::string const & section{v.first};
            if (section.find("DBModule") == 0u) {
                newDbModuleList.emplace_back(
                        DbModuleEntry{
                            v.second.get<std::string>("File"),
                            v.second.get<std::string>("Configuration", "")});
            } else if (section.find("DataSource") == 0u) {
                newDataSourceList.emplace_back(
                        DataSourceEntry{
                            v.second.get<std::string>("Name"),
                            v.second.get<std::string>("DBModule"),
                            v.second.get<std::string>("Configuration")});
            }
        }
    } catch (const pt::ini_parser_error & error) {
#if BOOST_VERSION <= 104200
        m_lastErrorMessage = error.what();
#else
        std::ostringstream o;
        o << error.message() << " [" << error.filename() << ":" << error.line() << "].";
        m_lastErrorMessage = o.str();
#endif
        return false;
    } catch (const pt::ptree_bad_data & error) {
        std::ostringstream o;
        o << "Bad data: " << error.what();
        m_lastErrorMessage = o.str();
        return false;
    } catch (const pt::ptree_bad_path & error) {
        std::ostringstream o;
        o << "Bad path: " << error.what();
        m_lastErrorMessage = o.str();
        return false;
    }

    m_dbModuleList = std::move(newDbModuleList);
    m_dataSourceList = std::move(newDataSourceList);

    return true;
}

} /* namespace sharemind { */
