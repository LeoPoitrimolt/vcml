/******************************************************************************
 *                                                                            *
 * Copyright 2018 Jan Henrik Weinstock                                        *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the "License");            *
 * you may not use this file except in compliance with the License.           *
 * You may obtain a copy of the License at                                    *
 *                                                                            *
 *     http://www.apache.org/licenses/LICENSE-2.0                             *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an "AS IS" BASIS,          *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 *                                                                            *
 ******************************************************************************/

#ifndef VCML_VSPSERVER_H
#define VCML_VSPSERVER_H

#include "vcml/common/includes.h"
#include "vcml/common/types.h"
#include "vcml/common/utils.h"
#include "vcml/common/report.h"
#include "vcml/common/aio.h"
#include "vcml/common/version.h"

#include "vcml/logging/logger.h"
#include "vcml/properties/property.h"
#include "vcml/debugging/rspserver.h"

#include "vcml/component.h"

namespace vcml { namespace debugging {

    class vspserver: public rspserver {
    private:
        string m_announce;

        string handle_none(const char* command);
        string handle_step(const char* command);
        string handle_cont(const char* command);
        string handle_info(const char* command);
        string handle_exec(const char* command);
        string handle_time(const char* command);
        string handle_dcyc(const char* command);
        string handle_rdgq(const char* command);
        string handle_wrgq(const char* command);
        string handle_geta(const char* command);
        string handle_seta(const char* command);
        string handle_quit(const char* command);
        string handle_vers(const char* command);

        void run_interruptible(const sc_time& duration);

    public:
        vspserver(u16 port);
        virtual ~vspserver();

        void start();
        void interrupt();
        void cleanup();

        virtual void handle_connect(const char* peer) override;
        virtual void handle_disconnect() override;
    };

}}

#endif
