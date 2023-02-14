#include <list>
#include <initializer_list>
#include <iterator>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <algorithm>

template <class KeyType, class ValueType, class Hash = std::hash<KeyType>>
class HashMap {
private:
    struct Node {
        std::pair<const KeyType, ValueType> x_;
        size_t psl;

        Node();
        Node(const Node& other);
        explicit Node(const std::pair<const KeyType, ValueType> x);
        Node& operator=(const Node& other);
    };

public:
    explicit HashMap();
    template <class Iterator>
    HashMap(Iterator begin, Iterator end);
    HashMap(std::initializer_list<std::pair<KeyType, ValueType>> list);
    explicit HashMap(Hash hasher);
    template <class Iterator>
    HashMap(Iterator begin, Iterator end, Hash hasher);
    HashMap(std::initializer_list<std::pair<KeyType, ValueType>> list, Hash hasher);
    HashMap(const HashMap& other);

    size_t size() const;  // NOLINT

    bool empty() const;  // NOLINT

    Hash hash_function() const;  // NOLINT

    void insert(const std::pair<KeyType, ValueType> x);  // NOLINT

    void erase(const KeyType v);  // NOLINT

    HashMap& operator= (const HashMap& other);

    class const_iterator {  // NOLINT
    public:
        const_iterator();
        const_iterator(const const_iterator& it);
        explicit const_iterator(typename std::vector<std::pair<size_t, Node*>*>::const_iterator begin, size_t id);

        const std::pair<const KeyType, ValueType>& operator*() const;
        const std::pair<const KeyType, ValueType>* operator->() const;

        const_iterator& operator++();
        const_iterator operator++(int);

        bool operator==(const const_iterator& other) const;
        bool operator!=(const const_iterator& other) const;

    private:
        typename std::vector<std::pair<size_t, Node*>*>::const_iterator begin_;
        size_t it_;
    };

    class iterator {  // NOLINT
    public:
        iterator();
        iterator(const iterator& it);
        explicit iterator(typename std::vector<std::pair<size_t, Node*>*>::iterator begin, size_t id);

        std::pair<const KeyType, ValueType>& operator*() const;
        std::pair<const KeyType, ValueType>* operator->() const;

        iterator& operator++();
        iterator operator++(int);

        bool operator==(const iterator& other) const;
        bool operator!=(const iterator& other) const;

    private:
        typename std::vector<std::pair<size_t, Node*>*>::iterator begin_;
        size_t it_;
    };

    const_iterator begin() const;  // NOLINT
    const_iterator end() const;    // NOLINT
    iterator begin();              // NOLINT
    iterator end();                // NOLINT

    iterator find(KeyType v);              // NOLINT
    const_iterator find(KeyType v) const;  // NOLINT

    ValueType& operator[](const KeyType v);
    const ValueType& at(const KeyType v) const;  // NOLINT

    void clear();  // NOLINT

private:
    size_t initial_bucket_count_ = 1;
    double max_load_factor_ = 0.25;
    std::vector<size_t> bucket_counts_ = {2,
                                          5,
                                          11,
                                          23,
                                          47,
                                          97,
                                          197,
                                          397,
                                          797,
                                          1597,
                                          3203,
                                          6421,
                                          12853,
                                          25717,
                                          51437,
                                          102877,
                                          205759,
                                          411527,
                                          823117,
                                          1646237,
                                          3292489,
                                          6584983,
                                          13169977,
                                          26339969,
                                          52679969,
                                          105359939,
                                          210719881,
                                          421439783,
                                          842879579,
                                          1685759167,
                                          3371518343,
                                          6743036717,
                                          13486073473,
                                          26972146961,
                                          53944293929,
                                          107888587883,
                                          215777175787,
                                          431554351609,
                                          863108703229,
                                          1726217406467,
                                          3452434812973,
                                          6904869625999,
                                          13809739252051,
                                          27619478504183,
                                          55238957008387,
                                          110477914016779,
                                          220955828033581,
                                          441911656067171,
                                          883823312134381,
                                          1767646624268779,
                                          3535293248537579,
                                          7070586497075177,
                                          14141172994150357,
                                          28282345988300791,
                                          56564691976601587,
                                          113129383953203213,
                                          226258767906406483,
                                          452517535812813007,
                                          905035071625626043,
                                          1810070143251252131,
                                          3620140286502504283,
                                          7240280573005008577};

    Hash hasher_;
    std::vector<std::pair<size_t, Node*>*> all_elements_;
    std::vector<Node> data_;
    std::vector<std::pair<size_t, Node*>> hash_map_;

    void Build();

    size_t get_hash(const KeyType v) const;  // NOLINT
};

template <class KeyType, class ValueType, class Hash>
HashMap<KeyType, ValueType, Hash>::Node::Node() {
}

template <class KeyType, class ValueType, class Hash>
HashMap<KeyType, ValueType, Hash>::Node::Node(const Node& other) : x_(other.x_), psl(other.psl) {
}

template <class KeyType, class ValueType, class Hash>
HashMap<KeyType, ValueType, Hash>::Node::Node(const std::pair<const KeyType, ValueType> x) : x_(x), psl(0) {
}

template <class KeyType, class ValueType, class Hash>
typename HashMap<KeyType, ValueType, Hash>::Node& HashMap<KeyType, ValueType, Hash>::Node::operator=(
    const HashMap::Node& other) {
    return (*this);
}

template <class KeyType, class ValueType, class Hash>
HashMap<KeyType, ValueType, Hash>::iterator::iterator(const HashMap::iterator& it) : begin_(it.begin_), it_(it.it_) {
}

template <class KeyType, class ValueType, class Hash>
HashMap<KeyType, ValueType, Hash>::iterator::iterator(typename std::vector<std::pair<size_t, Node*>*>::iterator begin,
                                                      size_t id)
    : begin_(begin), it_(id) {
}

template <class KeyType, class ValueType, class Hash>
std::pair<const KeyType, ValueType>& HashMap<KeyType, ValueType, Hash>::iterator::operator*() const {
    return (*(begin_ + it_))->second->x_;
}

template <class KeyType, class ValueType, class Hash>
std::pair<const KeyType, ValueType>* HashMap<KeyType, ValueType, Hash>::iterator::operator->() const {
    std::pair<const KeyType, ValueType>* tmp = &((*(begin_ + it_))->second->x_);
    return tmp;
}

template <class KeyType, class ValueType, class Hash>
typename HashMap<KeyType, ValueType, Hash>::iterator HashMap<KeyType, ValueType, Hash>::iterator::operator++(int) {
    auto it = *this;
    ++it_;
    return it;
}

template <class KeyType, class ValueType, class Hash>
bool HashMap<KeyType, ValueType, Hash>::iterator::operator==(const HashMap::iterator& other) const {
    return it_ == other.it_ && begin_ == other.begin_;
}

template <class KeyType, class ValueType, class Hash>
bool HashMap<KeyType, ValueType, Hash>::iterator::operator!=(const HashMap::iterator& other) const {
    return it_ != other.it_ || begin_ != other.begin_;
}

template <class KeyType, class ValueType, class Hash>
typename HashMap<KeyType, ValueType, Hash>::iterator& HashMap<KeyType, ValueType, Hash>::iterator::operator++() {
    ++it_;
    return (*this);
}

template <class KeyType, class ValueType, class Hash>
HashMap<KeyType, ValueType, Hash>::iterator::iterator() {
}

template <class KeyType, class ValueType, class Hash>
const std::pair<const KeyType, ValueType>& HashMap<KeyType, ValueType, Hash>::const_iterator::operator*() const {
    return (*(begin_ + it_))->second->x_;
}

template <class KeyType, class ValueType, class Hash>
const std::pair<const KeyType, ValueType>* HashMap<KeyType, ValueType, Hash>::const_iterator::operator->() const {
    const std::pair<const KeyType, ValueType>* tmp = &((*(begin_ + it_))->second->x_);
    return tmp;
}

template <class KeyType, class ValueType, class Hash>
typename HashMap<KeyType, ValueType, Hash>::const_iterator&
HashMap<KeyType, ValueType, Hash>::const_iterator::operator++() {
    ++it_;
    return (*this);
}

template <class KeyType, class ValueType, class Hash>
typename HashMap<KeyType, ValueType, Hash>::const_iterator
HashMap<KeyType, ValueType, Hash>::const_iterator::operator++(int) {
    auto it = *this;
    ++it_;
    return it;
}

template <class KeyType, class ValueType, class Hash>
bool HashMap<KeyType, ValueType, Hash>::const_iterator::operator==(const HashMap::const_iterator& other) const {
    return it_ == other.it_ && begin_ == other.begin_;
}

template <class KeyType, class ValueType, class Hash>
bool HashMap<KeyType, ValueType, Hash>::const_iterator::operator!=(const HashMap::const_iterator& other) const {
    return it_ != other.it_ || begin_ != other.begin_;
}

template <class KeyType, class ValueType, class Hash>
HashMap<KeyType, ValueType, Hash>::const_iterator::const_iterator(const HashMap::const_iterator& it)
    : begin_(it.begin_), it_(it.it_) {
}

template <class KeyType, class ValueType, class Hash>
HashMap<KeyType, ValueType, Hash>::const_iterator::const_iterator(
    typename std::vector<std::pair<size_t, Node*>*>::const_iterator begin, size_t id)
    : begin_(begin), it_(id) {
}

template <class KeyType, class ValueType, class Hash>
HashMap<KeyType, ValueType, Hash>::const_iterator::const_iterator() {
}

template <class KeyType, class ValueType, class Hash>
HashMap<KeyType, ValueType, Hash>::HashMap() {
    hash_map_.resize(initial_bucket_count_, std::make_pair(initial_bucket_count_, nullptr));
}

template <class KeyType, class ValueType, class Hash>
HashMap<KeyType, ValueType, Hash>::HashMap(std::initializer_list<std::pair<KeyType, ValueType>> list) {
    hash_map_.resize(initial_bucket_count_, std::make_pair(initial_bucket_count_, nullptr));
    for (auto x : list) {
        insert(x);
    }
}

template <class KeyType, class ValueType, class Hash>
HashMap<KeyType, ValueType, Hash>::HashMap(Hash hasher) : hasher_(hasher) {
    hash_map_.resize(initial_bucket_count_, std::make_pair(initial_bucket_count_, nullptr));
}

template <class KeyType, class ValueType, class Hash>
HashMap<KeyType, ValueType, Hash>::HashMap(std::initializer_list<std::pair<KeyType, ValueType>> list, Hash hasher)
    : hasher_(hasher) {
    hash_map_.resize(initial_bucket_count_, std::make_pair(initial_bucket_count_, nullptr));
    for (auto x : list) {
        insert(x);
    }
}

template <class KeyType, class ValueType, class Hash>
size_t HashMap<KeyType, ValueType, Hash>::size() const {
    return all_elements_.size();
}

template <class KeyType, class ValueType, class Hash>
bool HashMap<KeyType, ValueType, Hash>::empty() const {
    return (all_elements_.size() == 0);
}

template <class KeyType, class ValueType, class Hash>
Hash HashMap<KeyType, ValueType, Hash>::hash_function() const {
    return hasher_;
}

template <class KeyType, class ValueType, class Hash>
void HashMap<KeyType, ValueType, Hash>::insert(const std::pair<KeyType, ValueType> x) {
    auto it = find(x.first);
    if (it != end()) {
        return;
    }
    if (all_elements_.size() + 1 > max_load_factor_ * hash_map_.size() || data_.size() == data_.capacity()) {
        Build();
    }

    size_t pos = get_hash(x.first);
    data_.emplace_back(x);
    all_elements_.push_back(nullptr);
    std::pair<size_t, Node*> now = std::make_pair(all_elements_.size() - 1, &data_.back());
    while (hash_map_[pos].second != nullptr) {
        if (now.second->psl > hash_map_[pos].second->psl) {
            swap(now, hash_map_[pos]);
            swap(all_elements_[now.first], all_elements_[hash_map_[pos].first]);
        }
        ++now.second->psl;
        ++pos;
        if (pos == hash_map_.size()) {
            pos = 0;
        }
    }
    hash_map_[pos] = now;
    all_elements_[hash_map_[pos].first] = &hash_map_[pos];
}

template <class KeyType, class ValueType, class Hash>
void HashMap<KeyType, ValueType, Hash>::erase(const KeyType v) {
    size_t pos = get_hash(v);
    while (hash_map_[pos].second != nullptr && !((hash_map_[pos].second->x_).first == v)) {  // NOLINT
        ++pos;
        if (pos == hash_map_.size()) {
            pos = 0;
        }
    }
    if (hash_map_[pos].second != nullptr) {
        all_elements_.back()->first = hash_map_[pos].first;
        swap(all_elements_[hash_map_[pos].first], all_elements_.back());
        all_elements_.pop_back();
        hash_map_[pos].second = nullptr;
        while (true) {
            size_t nxt = pos + 1;
            if (nxt == hash_map_.size()) {
                nxt = 0;
            }
            if (hash_map_[nxt].second != nullptr && hash_map_[nxt].second->psl != 0) {
                all_elements_[hash_map_[nxt].first] = &hash_map_[pos];
                swap(hash_map_[nxt], hash_map_[pos]);
                --hash_map_[pos].second->psl;
                pos = nxt;
            } else {
                break;
            }
        }
    }
}

template <class KeyType, class ValueType, class Hash>
void HashMap<KeyType, ValueType, Hash>::Build() {
    size_t new_size_id = 0;
    while (all_elements_.size() + 1 > bucket_counts_[new_size_id] * max_load_factor_) {
        ++new_size_id;
    }
    size_t new_size = bucket_counts_[new_size_id];
    std::vector<std::pair<const KeyType, ValueType>> tmp;
    for (auto c : all_elements_) {
        tmp.push_back(c->second->x_);
    }
    all_elements_.clear();
    data_.clear();
    data_.reserve(new_size);
    hash_map_.clear();
    hash_map_.resize(new_size, std::make_pair(0, nullptr));
    for (auto& x : tmp) {
        insert(x);
    }
}

template <class KeyType, class ValueType, class Hash>
typename HashMap<KeyType, ValueType, Hash>::const_iterator HashMap<KeyType, ValueType, Hash>::begin() const {
    return const_iterator(all_elements_.begin(), 0);
}

template <class KeyType, class ValueType, class Hash>
typename HashMap<KeyType, ValueType, Hash>::const_iterator HashMap<KeyType, ValueType, Hash>::end() const {
    return const_iterator(all_elements_.begin(), all_elements_.size());
}

template <class KeyType, class ValueType, class Hash>
typename HashMap<KeyType, ValueType, Hash>::iterator HashMap<KeyType, ValueType, Hash>::begin() {
    return iterator(all_elements_.begin(), 0);
}

template <class KeyType, class ValueType, class Hash>
typename HashMap<KeyType, ValueType, Hash>::iterator HashMap<KeyType, ValueType, Hash>::end() {
    return iterator(all_elements_.begin(), all_elements_.size());
}

template <class KeyType, class ValueType, class Hash>
typename HashMap<KeyType, ValueType, Hash>::iterator HashMap<KeyType, ValueType, Hash>::find(const KeyType v) {
    size_t pos = get_hash(v);
    while (hash_map_[pos].second != nullptr && !((hash_map_[pos].second->x_).first == v)) {  // NOLINT
        ++pos;
        if (pos == hash_map_.size()) {
            pos = 0;
        }
    }
    if (hash_map_[pos].second == nullptr) {
        return end();
    } else {
        return iterator(all_elements_.begin(), hash_map_[pos].first);
    }
}

template <class KeyType, class ValueType, class Hash>
typename HashMap<KeyType, ValueType, Hash>::const_iterator HashMap<KeyType, ValueType, Hash>::find(
    const KeyType v) const {
    size_t pos = get_hash(v);
    while (hash_map_[pos].second != nullptr && !((hash_map_[pos].second->x_).first == v)) {  // NOLINT
        ++pos;
        if (pos == hash_map_.size()) {
            pos = 0;
        }
    }
    if (hash_map_[pos].second == nullptr) {
        return end();
    } else {
        return const_iterator(all_elements_.begin(), hash_map_[pos].first);
    }
}

template <class KeyType, class ValueType, class Hash>
template <class Iterator>
HashMap<KeyType, ValueType, Hash>::HashMap(Iterator begin, Iterator end) {
    hash_map_.resize(initial_bucket_count_, std::make_pair(0, nullptr));
    for (auto it = begin; it != end; ++it) {
        insert(*it);
    }
}

template <class KeyType, class ValueType, class Hash>
template <class Iterator>
HashMap<KeyType, ValueType, Hash>::HashMap(Iterator begin, Iterator end, Hash hasher) : hasher_(hasher) {
    hash_map_.resize(initial_bucket_count_, std::make_pair(0, nullptr));
    for (auto it = begin; it != end; ++it) {
        insert(*it);
    }
}

template <class KeyType, class ValueType, class Hash>
ValueType& HashMap<KeyType, ValueType, Hash>::operator[](const KeyType v) {
    insert(std::make_pair(v, ValueType()));
    iterator it = find(v);
    return it->second;
}

template <class KeyType, class ValueType, class Hash>
const ValueType& HashMap<KeyType, ValueType, Hash>::at(const KeyType v) const {
    const_iterator it = find(v);
    if (it == end()) {
        throw std::out_of_range("no such key");
    }
    return it->second;
}

template <class KeyType, class ValueType, class Hash>
HashMap<KeyType, ValueType, Hash>::HashMap(const HashMap& other) {
    std::vector<std::pair<const KeyType, ValueType>> tmp;
    for (auto c : other.all_elements_) {
        tmp.push_back((*c).second->x_);
    }
    size_t sz = other.hash_map_.size();
    hash_map_.clear();
    data_.clear();
    all_elements_.clear();
    hash_map_.resize(sz, std::make_pair(0, nullptr));
    data_.reserve(sz);
    for (auto c : tmp) {
        insert(c);
    }
}

template <class KeyType, class ValueType, class Hash>
void HashMap<KeyType, ValueType, Hash>::clear() {
    std::vector<std::pair<const KeyType, ValueType>> tmp;
    for (auto c : all_elements_) {
        tmp.push_back((*c).second->x_);
    }
    for (auto c : tmp) {
        erase(c.first);
    }
}

template <class KeyType, class ValueType, class Hash>
size_t HashMap<KeyType, ValueType, Hash>::get_hash(const KeyType v) const {
    return (hasher_(v) * 30011 + 179) % hash_map_.size();
}
template <class KeyType, class ValueType, class Hash>
HashMap<KeyType, ValueType, Hash>& HashMap<KeyType, ValueType, Hash>::operator=(const HashMap& other) {
    std::vector<std::pair<const KeyType, ValueType>> tmp;
    for (auto c : other.all_elements_) {
        tmp.push_back((*c).second->x_);
    }
    size_t sz = other.hash_map_.size();
    hash_map_.clear();
    data_.clear();
    all_elements_.clear();
    hash_map_.resize(sz, std::make_pair(0, nullptr));
    data_.reserve(sz);
    for (auto c : tmp) {
        insert(c);
    }
    return (*this);
}
