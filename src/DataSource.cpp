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

#include <cassert>

#include "DataSource.h"


#ifdef __cplusplus
extern "C" {
#endif

const char * SharemindDataSource_name(SharemindDataSource * src) {
    assert(src);
    return sharemind::DataSource::fromWrapper(*src).name().c_str();
}

const char * SharemindDataSource_module(SharemindDataSource * src) {
    assert(src);
    return sharemind::DataSource::fromWrapper(*src).module().c_str();
}

const char * SharemindDataSource_conf(SharemindDataSource * src) {
    assert(src);
    return sharemind::DataSource::fromWrapper(*src).conf().c_str();
}

#ifdef __cplusplus
} /* extern "C" { */
#endif

namespace sharemind {

DataSource::DataSource(std::string name, std::string module, std::string conf)
    : ::SharemindDataSource{&SharemindDataSource_name,
                            &SharemindDataSource_module,
                            &SharemindDataSource_conf}
    , m_name(std::move(name))
    , m_module(std::move(module))
    , m_conf(std::move(conf))
{}

} /* namespace sharemind { */
