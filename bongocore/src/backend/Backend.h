#pragma once

#include <atomic>

#include "backend/IProcessor.h"
#include "common/OperationResults.h"
#include "common/Shard.h"
#include "common/StreamCommands.h"
#include "common/types.h"

namespace bongodb::Backend {
class TBackend {
public:
    Common::TGetResult Get(const Common::TKey& key);
    Common::TRemoveResult Remove(const Common::TKey& key);
    Common::TTruncateResult Truncate();
    Common::TPutResult Put(Common::TKey&& key, Common::TValue&& value);
    void Stream(Common::IStreamCommand&& command, Common::TVersion&& version);

    bool IsReady();
    bool Prepare();

private:
    bool IsForCurrentShard(const Common::TKey& key);
    std::shared_ptr<Common::TReplica> ChooseReplica(const Common::TKey& key);

    Common::TShards Shards;
    std::unique_ptr<IProcessor> Processor;
    /// State can change in time (shard master changed and etc) and prepare needs to be called
    std::atomic<bool> Ready = false;
};
}  // namespace bongodb::Backend
