/**
*    Copyright (C) 2008 10gen Inc.
*
*    This program is free software: you can redistribute it and/or  modify
*    it under the terms of the GNU Affero General Public License, version 3,
*    as published by the Free Software Foundation.
*
*    This program is distributed in the hope that it will be useful,b
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU Affero General Public License for more details.
*
*    You should have received a copy of the GNU Affero General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*    As a special exception, the copyright holders give permission to link the
*    code of portions of this program with the OpenSSL library under certain
*    conditions as described in each individual source file and distribute
*    linked combinations including the program with the OpenSSL library. You
*    must comply with the GNU Affero General Public License in all respects for
*    all of the code used other than as permitted herein. If you modify file(s)
*    with this exception, you may extend this exception to your version of the
*    file(s), but you are not obligated to do so. If you do not wish to do so,
*    delete this exception statement from your version. If you delete this
*    exception statement from all source files in the program, then also delete
*    it in the license file.
*/

#include "mongo/pch.h"

#include "mongo/db/repl/heartbeat_info.h"

#include "mongo/util/fail_point_service.h"

namespace mongo {

    unsigned int HeartbeatInfo::numPings;

    HeartbeatInfo::HeartbeatInfo() : 
        hbstate(MemberState::RS_UNKNOWN), 
        health(-1.0), 
        upSince(0),
        downSince(0), 
        lastHeartbeat(0),
        lastHeartbeatRecv(0),
        skew(INT_MIN), 
        authIssue(false), 
        ping(0), 
        _id(0xffffffff) { 
    }

    HeartbeatInfo::HeartbeatInfo(unsigned id) :
        hbstate(MemberState::RS_UNKNOWN),
        health(-1.0),
        upSince(0),
        downSince(0),
        lastHeartbeat(0),
        lastHeartbeatRecv(0),
        skew(INT_MIN),
        authIssue(false),
        ping(0),
        _id(id) {
    }

    bool HeartbeatInfo::changed(const HeartbeatInfo& old) const {
        return health != old.health || hbstate != old.hbstate;
    }

    void HeartbeatInfo::updateFromLastPoll(const HeartbeatInfo& newInfo) {
        hbstate = newInfo.hbstate;
        health = newInfo.health;
        upSince = newInfo.upSince;
        downSince = newInfo.downSince;
        lastHeartbeat = newInfo.lastHeartbeat;
        lastHeartbeatMsg = newInfo.lastHeartbeatMsg;
        // Note: lastHeartbeatRecv is updated through CmdReplSetHeartbeat::run().

        syncingTo = newInfo.syncingTo;
        opTime = newInfo.opTime;
        skew = newInfo.skew;
        authIssue = newInfo.authIssue;
        ping = newInfo.ping;
        electionTime = newInfo.electionTime;
    }


} // namespace mongo
