#include <cerrno>
#include <cstdlib>
#include <cassert>

#include "core/SelectDispatcher.h"
#include "core/Channel.h"
#include "log/Log.h"

namespace lite_http {

void SelectDispatcher::Init() {
  FD_ZERO(&readset_);
  FD_ZERO(&writeset_);
  FD_ZERO(&exset_);
}

void SelectDispatcher::Add(Channel *channel) {
  int fd = channel->Fd();
  LOG_INFO("channel Add %d", fd);
  map_[fd] = channel;

  if (channel->Readable())
    FD_SET(fd, &readset_);
  if (channel->Writable())
    FD_SET(fd, &writeset_);
}

void SelectDispatcher::Update(Channel *channel) {
  if (map_.count(channel->Fd()) == 0) {
    Add(channel);
  } else {
    int fd = channel->Fd();
    LOG_INFO("channel Update %d", fd);
    if (channel->Readable())
      FD_SET(fd, &readset_);
    else
      FD_CLR(fd, &readset_);

    if (channel->Writable())
      FD_SET(fd, &writeset_);
    else
      FD_CLR(fd, &writeset_);
  }
}

void SelectDispatcher::Del(Channel *channel) {
  assert(channel->IsNone());
  int fd = channel->Fd();
  FD_CLR(fd, &writeset_);
  FD_CLR(fd, &readset_);
  FD_CLR(fd, &exset_);
  LOG_INFO("channel Del %d", fd);
  map_.erase(fd);
}

void SelectDispatcher::Dispatch(struct timeval *timeval, std::vector<Channel *> &channels) {
  if (map_.empty()) {
    LOG_WARN("no Fd to select.");
    return;
  }

  readmask_ = readset_;
  writemask_ = writeset_;
  exmask_ = exset_;
  LOG_INFO("max Fd = %d, Start select.", map_.rbegin()->first);
  if (select(map_.rbegin()->first + 1, &readmask_, &writemask_, &exmask_, timeval) < 0) {
    LOG_WARN("select error.");
    return;
  }

  for (auto it = map_.begin(); it != map_.end(); ++it) {
    int fd = it->first;
    Channel *ch = it->second;
    assert(ch->Fd() == fd);
    int revent = kEventNone;
    if (FD_ISSET(fd, &readmask_))
      revent |= kEventRead;

    if (FD_ISSET(fd, &writemask_))
      revent |= kEventWrite;
    ch->SetRevent(revent);
    if (revent == kEventNone) continue;
    channels.push_back(ch);
  }
}

void SelectDispatcher::Clear() {
  map_.clear();
  Init();
}

} // namespace
