#pragma once

#include "../Core/InternalInclude.hpp"

namespace Syrius{

    template<typename K, typename D>
    class KeyVector{
            public:

            KeyVector() = default;

            explicit KeyVector(uint32 size){
                m_Data.reserve(size);
            }

            ~KeyVector() = default;

            void insert(const K& key, const D& data){
                SR_PRECONDITION(m_KeyIndexMap.find(key) == m_KeyIndexMap.end(), "[KeyVector]: Key (%ull) already exists", key)

                m_Data.push_back(data);
                m_KeyIndexMap.insert({key, m_Data.size() - 1});
            }

            template<typename... Args>
            void emplace(const K& key, Args&&... args){
                SR_PRECONDITION(m_KeyIndexMap.find(key) == m_KeyIndexMap.end(), "[KeyVector]: Key (%ull) already exists", key)

                m_Data.emplace_back(std::forward<Args>(args)...);
                m_KeyIndexMap.insert({key, m_Data.size() - 1});
            }

            const D& get(const K& key) const {
                SR_PRECONDITION(m_KeyIndexMap.find(key) != m_KeyIndexMap.end(), "[KeyVector]: cannot retrieve element, key (%ull) does not exists", key);

                return m_Data[m_KeyIndexMap[key]];
            }

            D& get(const K& key){
                SR_PRECONDITION(m_KeyIndexMap.find(key) != m_KeyIndexMap.end(), "[KeyVector]: cannot retrieve element, key (%ull) does not exists", key);

                return m_Data[m_KeyIndexMap[key]];
            }

            const D& operator[](const K& key) const{
                SR_PRECONDITION(m_KeyIndexMap.find(key) != m_KeyIndexMap.end(), "[KeyVector]: cannot retrieve element, key (%ull) does not exists", key);

                return m_Data[m_KeyIndexMap[key]];
            }

            D& operator[](const K& key){
                SR_PRECONDITION(m_KeyIndexMap.find(key) != m_KeyIndexMap.end(), "[KeyVector]: cannot retrieve element, key (%ull) does not exists", key);

                return m_Data[m_KeyIndexMap[key]];
            }

            void remove(const K& key){
                SR_PRECONDITION(m_KeyIndexMap.find(key) != m_KeyIndexMap.end(), "[KeyVector]: cannot remove element, key (%ull) does not exists", key);

                auto lastDataIndex = m_Data.size() - 1;
                auto currentDataIndex = m_KeyIndexMap[key];
                K lastElementKey;
                for (const auto& lastKey : m_KeyIndexMap){
                    if (lastDataIndex == lastKey.second){
                        lastElementKey = lastKey.first;
                        break;
                    }
                }
                // switch last and current element from place
                std::swap(m_Data[currentDataIndex], m_Data[lastDataIndex]);
                m_Data.pop_back();
                m_KeyIndexMap.erase(key);
                // update index
                m_KeyIndexMap[lastElementKey] = currentDataIndex;
            }

            typename std::vector<D>::iterator begin(){
                return m_Data.begin();
            }

            typename std::vector<D>::iterator begin() const {
                return m_Data.begin();
            }

            typename std::vector<D>::iterator end(){
                return m_Data.end();
            }

            typename std::vector<D>::iterator end() const {
                return m_Data.end();
            }

            bool has(const K& key) const {
                return m_KeyIndexMap.find(key) != m_KeyIndexMap.end();
            }

            [[nodiscard]] Size getSize() const{
                return m_Data.size();
            }

            const std::vector<D>& getData() const{
                return m_Data;
            }

            private:
            std::vector<D> m_Data;
            std::unordered_map<K, uint64> m_KeyIndexMap;
    };

}