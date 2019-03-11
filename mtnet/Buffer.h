/*
Buffer相关操作封装  发送数据时用来存储数据（每次发送的数据可能不能一次性发完，需要做一下缓冲；
接受数据也是同理）
*/
#ifndef MYNET_MTNET_BUFF_H
#define MYNET_MTNET_BUFF_H

#include <algorithm>
#include <string>
#include <vector>

#include <assert.h>

namespace mtnet
{
	class Buffer
	{
	public:
		static const size_t kCheapPrepend = 8;
		static const size_t kInitialSize = 1024;

		Buffer()
			:buffer_(kCheapPrepend + kInitialSize),
			readerIndex_(kCheapPrepend),
			writeIndex_(kCheapPrepend)
		{
			assert(readableBytes() == 0);
			assert(writeableBytes() == kInitialSize);
			assert(prependableBytes() == kCheapPrepend);
		}
		void swap(Buffer& rhs)
		{
			buffer_.swap(rhs.buffer_);
			std::swap(readerIndex_,rhs.readerIndex_);
			std::swap(writeIndex_,rhs.writeIndex_);
		}

		size_t readableBytes() const
		{
			return writeIndex_ - readerIndex_;
		}

		size_t writeableBytes() const
		{
			return buffer_.size() - writeIndex_;
		}

		size_t prependableBytes() const
		{
			return readerIndex_;
		}

		const char* peek() const
		{
			return begin() + readerIndex_;
		}

		void retrieve(size_t len)
		{
			assert(len <= readableBytes());
			readerIndex_ += len;
		}

		void retrieveUntil(const char* end)
		{
			assert(peek() <= end);
			assert(end <= beginWrite());
			retrieve(end - peek());
		}

		void retrieveAll()
		{
			readerIndex_ = kCheapPrepend;
			writeIndex_ = kCheapPrepend;
		}

		std::string retrieveAsString()
		{
			std::string str(peek(),readableBytes());
			retrieveAll();
			return str;
		}

		void append(const std::string& str)
		{
			append(str.data(),str.length());
		}

		void append(const char* data, size_t len)
		{
			ensureWritableBytes(len);
			std::copy(data, data+len,beginWrite());
			hasWriteten(len);
		}

		void append(const void* data, size_t len)
		{
			append(static_cast<const char*>(data), len);
		}

		void ensureWritableBytes(size_t len)
		{
			if (writeableBytes() < len)
			{
				makeSpace(len);
			}
			assert(writeableBytes() >= len);
		}

		char* beginWrite()
		{
			return begin() + writeIndex_;
		}

		const char* beginWrite() const
		{
			return begin() + writeIndex_;
		}

		void hasWriteten(size_t len)
		{
			writeIndex_ += len;
		}

		void prepend(const void* data, size_t len)
		{
			assert(len <= prependableBytes());
			readerIndex_ -= len;
			const char* d = static_cast<const char*>(data);
			std::copy(d, d+len, begin()+readerIndex_);
		}

		void shrink(size_t reserve)
		{
			std::vector<char> buf(kCheapPrepend + readableBytes() + reserve);
			std::copy(peek(), peek()+readableBytes(), buf.begin()+kCheapPrepend);
			buf.swap(buffer_);
		}

		//将数据导入到buffer中，返回read的结果，并将错误码保存在savedErrno中
		ssize_t readFd(int fd,int* savedErrno);

	protected:
	private:
		char* begin()
		{
			return &*buffer_.begin();
		}

		const char* begin() const
		{
			return &*buffer_.begin();
		}

		void makeSpace(size_t len)
		{
			if (writeableBytes() + prependableBytes() < len + kCheapPrepend)
			{
				buffer_.resize(writeIndex_ + len);
			} 
			else
			{
				//移动可读数据到缓冲前面
				assert(kCheapPrepend < readerIndex_);
				size_t readable = readableBytes();
				std::copy(begin() + readerIndex_,
					begin() + writeIndex_,
					begin() + kCheapPrepend);
				readerIndex_ = kCheapPrepend;
				writeIndex_ = readerIndex_ + readable;
				assert(readable == readableBytes());
			}
		}

		std::vector<char> buffer_;
		size_t readerIndex_;
		size_t writeIndex_;
	};
}

#endif //MYNET_MTNET_BUFF_H