/* Copyright (c) 2018 - present, VE Software Inc. All rights reserved
 *
 * This source code is licensed under Apache 2.0 License
 *  (found in the LICENSE.Apache file in the root directory)
 */

#ifndef VGRAPHD_SESSIONMANAGER_H_
#define VGRAPHD_SESSIONMANAGER_H_

#include "base/Base.h"
#include "base/StatusOr.h"
#include "vgraphd/ClientSession.h"
#include "thread/GenericWorker.h"

/**
 * SessionManager manages the client sessions, e.g. create new, find existing and drop expired.
 */

namespace vesoft {
namespace vgraph {

class SessionManager final {
public:
    SessionManager();
    ~SessionManager();

    using SessionPtr = std::shared_ptr<ClientSession>;
    /**
     * Find an existing session
     */
    StatusOr<SessionPtr> findSession(int64_t id);
    /**
     * Create a new session
     */
    SessionPtr createSession();
    /**
     * Remove a session
     */
    SessionPtr removeSession(int64_t id);

private:
    /**
     * Generate a non-zero number
     */
    int64_t newSessionId();

    void reclaimExpiredSessions();

private:
    std::atomic<int64_t>                        nextId_{0};
    folly::RWSpinLock                           rwlock_;        // TODO(dutor) writer might starve
    std::unordered_map<int64_t, SessionPtr>     activeSessions_;
    std::unique_ptr<thread::GenericWorker>      scavenger_;
};

}   // namespace vgraph
}   // namespace vesoft


#endif  // VGRAPHD_SESSIONMANAGER_H_