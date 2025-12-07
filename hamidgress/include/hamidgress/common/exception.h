#pragma once

#include <stdexcept>
#include <string>

namespace hamidgress {

class DatabaseException : public std::runtime_error {
public:
    explicit DatabaseException(const std::string& msg)
        : std::runtime_error(msg) {}
};

class IOException : public DatabaseException {
public:
    explicit IOException(const std::string& msg)
        : DatabaseException("I/O Error: " + msg) {}
};

class BufferPoolException : public DatabaseException {
public:
    explicit BufferPoolException(const std::string& msg)
        : DatabaseException("Buffer Pool Error: " + msg) {}
};

class BufferPoolFullException : public BufferPoolException {
public:
    BufferPoolFullException()
        : BufferPoolException("Buffer pool is full, no victim available") {}
};

class CatalogException : public DatabaseException {
public:
    explicit CatalogException(const std::string& msg)
        : DatabaseException("Catalog Error: " + msg) {}
};

class ExecutionException : public DatabaseException {
public:
    explicit ExecutionException(const std::string& msg)
        : DatabaseException("Execution Error: " + msg) {}
};

class TransactionException : public DatabaseException {
public:
    explicit TransactionException(const std::string& msg)
        : DatabaseException("Transaction Error: " + msg) {}
};

class ParserException : public DatabaseException {
public:
    explicit ParserException(const std::string& msg)
        : DatabaseException("Parser Error: " + msg) {}
};

}
