#include "Buffer.h"
#include "SocketsOps.h"
#include "Logging.h"

#include <errno.h>
#include <memory.h>
#include <sys/uio.h>

using namespace mtnet;

ssize_t Buffer::readFd(int fd,int* savedErrno)
{
	char extrabuf[65536];
	struct iovec vec[2];
	const size_t writable = writeableBytes();
	vec[0].iov_base = begin() + writeIndex_;
	vec[0].iov_len = writable;
	vec[1].iov_base = extrabuf;
	vec[1].iov_len = sizeof(extrabuf);
	const ssize_t n = readv(fd,vec,2);
	if (n < 0)
	{
		*savedErrno = errno;
	}
	else if (static_cast<size_t>(n) <= writable)
	{
		writeIndex_ += n;
	}
	else
	{
		writeIndex_ = buffer_.size();
		append(extrabuf,n - writable);
	}
	return n;
}
