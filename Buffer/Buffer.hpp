#pragma once
#include <experimental/net>
#include <string>
#include <vector>
#include <iostream>

namespace bre {

    class Buffer {
    public:
        explicit Buffer(int initBuffersize = 1024) : buffer(initBuffersize), readPost(0), writePost(0){};

        ~Buffer() = default;

        Buffer(const Buffer &) = delete;           //禁止拷贝构造
        Buffer &operator=(const Buffer &) = delete;//禁止拷贝复制

        //移动构造
        Buffer(Buffer &&other) noexcept : buffer(std::move(other.buffer)), readPost(other.readPost), writePost(other.writePost) {
            other.readPost = 0;
            other.writePost = 0;
        }

        //移动赋值
        Buffer &operator=(Buffer &&other) noexcept {
            buffer = std::move(other.buffer);
            readPost = other.readPost;
            writePost = other.writePost;
            other.readPost = 0;
            other.writePost = 0;
        }

        size_t WritableBytes() const {
            return buffer.size() - writePost;
        }

        size_t ReadableBytes() const {
            return writePost - readPost;
        }

        const char *Peek() const {
            return buffer.data() + readPost;
        }

        std::string Retrieve(size_t len) {
            if (len > ReadableBytes()) {
                throw std::out_of_range("Buffer::Retrieve: len is too large!");
            }
            std::string ret = std::string(Peek(), len);
            readPost += len;
            return ret;
        }

        std::string RetrieveUntil(const std::string end) {
            if (end.size() > ReadableBytes()) {
                return "";
            }
            const std::size_t pos = std::string(buffer.data() + readPost, buffer.data() + writePost).find(end);
            if (pos == std::string::npos) {
                return "";
            }
            return Retrieve(pos + end.size());
        }

        std::string RetrieveAll() {
            std::string ret = std::string(Peek(), ReadableBytes());
            clear();
            return ret;
        }

        std::string ToString() {
            return std::string(Peek(), ReadableBytes());
        }

        void Append(const std::string &str) {
            Append(str.data(), str.size());
        }

        void Append(const void *data, size_t len) {
            Append(static_cast<const char *>(data), len);
        }

        void Append(const bre::Buffer &buff) {
            Append(buff.Peek(), buff.ReadableBytes());
        }

        void Append(const char *str, const size_t len) {
            if (WritableBytes() < len) {
                expandBuffer(len);
            }
            std::copy(str, str + len, buffer.data() + writePost);
            writePost += len;
        }

        void clear() {
            readPost = 0;
            writePost = 0;
            for (auto &item: buffer) {
                item = 0;
            }
        }

    private:
        void expandBuffer(size_t len) {
            if (WritableBytes() + readPost < len) {
                buffer.resize(writePost + len);
            } else {
                size_t readable = ReadableBytes();
                std::copy(buffer.data() + readPost, buffer.data() + writePost, buffer.data());
                readPost = 0;
                writePost = readPost + readable;
            }
        }

    private:
        std::vector<char> buffer;
        size_t readPost;
        size_t writePost;
    };


};// namespace bre
