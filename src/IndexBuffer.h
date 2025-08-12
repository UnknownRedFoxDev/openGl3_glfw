#pragma once

class IndexBuffer {
    private:
        unsigned int mRendereID;
        unsigned int mCount;
    public:
        IndexBuffer(const unsigned int* data, unsigned int count);
        ~IndexBuffer();

        void Bind() const;
        void Unbind() const;

        inline unsigned int GetCount() const { return mCount; }
};
