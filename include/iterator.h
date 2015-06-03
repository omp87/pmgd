#pragma once

#include <stddef.h>
#include <functional>
#include "exception.h"
#include "stringid.h"
#include "property.h"

#ifdef _MSC_VER
// Disable the multiple copy constructor warning.
#pragma warning(disable : 4521)
// Disable the empty array in struct/union warning.
#pragma warning(disable : 4200)
#endif

namespace Jarvis {
    enum Disposition { DontPass, Pass, Stop, PassStop, Prune, PassPrune };

    template <typename R> class IteratorImplIntf {
    public:
        typedef R Ref_type;
        virtual ~IteratorImplIntf() { }
        virtual operator bool() const = 0;
        virtual const Ref_type &operator*() const = 0;
        virtual const Ref_type *operator->() const = 0;
        virtual Ref_type &operator*() = 0;
        virtual Ref_type *operator->() = 0;
        virtual bool next() = 0;
    };

    template <typename Impl> class Iterator {
    protected:
        Impl *_impl;
        void done() { delete _impl; _impl = NULL; }

        explicit Iterator(Impl *i)
            : _impl(i)
        {
            if (_impl && !bool(*_impl))
                done();
        }

    public:
        Iterator(const Iterator &) = delete;
        typedef Impl Impl_type;
        typedef typename Impl::Ref_type Ref_type;

        Iterator(Iterator &orig)
        {
            _impl = orig._impl;
            orig._impl = NULL;
        }

        Iterator(Iterator &&orig)
        {
            _impl = orig._impl;
            orig._impl = NULL;
        }

        ~Iterator() { delete _impl; }

        operator bool() const { return _impl != NULL; }
        const Ref_type &operator*() const
            { if (!_impl) throw Exception(NullIterator); return (*_impl).operator*(); }
        const Ref_type *operator->() const
            { if (!_impl) throw Exception(NullIterator); return (*_impl).operator->(); }
        Ref_type &operator*()
            { if (!_impl) throw Exception(NullIterator); return (*_impl).operator*(); }
        Ref_type *operator->()
            { if (!_impl) throw Exception(NullIterator); return (*_impl).operator->(); }
        void next() { if (_impl) if (!_impl->next()) done(); }

        void process(std::function<void(Ref_type &)> f)
        {
            while (operator bool()) {
                f(operator*());
                next();
            }
        }
    };
};

namespace Jarvis {
    class Node;
    typedef Node NodeRef;
    typedef IteratorImplIntf<NodeRef> NodeIteratorImplIntf;

    class NodeIterator : public Iterator<NodeIteratorImplIntf> {
    public:
        explicit NodeIterator(NodeIteratorImplIntf *i)
            : Iterator<NodeIteratorImplIntf>(i) { }

        NodeIterator filter(const PropertyPredicate &pp);
        NodeIterator filter(std::function<Disposition(const Ref_type &)> f);
    };
};

namespace Jarvis {
    class Allocator;
    class PropertyList;
    class TransactionImpl;

    class PropertyRef {
        uint8_t *_chunk;
        unsigned _offset;

        friend class PropertyList;
        friend class PropertyListIteratorImpl;

        struct BlobRef;

        enum { p_unused, p_end, p_link,
               p_novalue, p_boolean_false, p_boolean_true,
               p_integer, p_float, p_time, p_string, p_string_ptr, p_blob };

        unsigned chunk_end() const { return _chunk[0]; }
        unsigned chunk_size() const { return chunk_end() + 1; }

        uint8_t &type_size() { return _chunk[_offset]; }
        const uint8_t &type_size() const
            { return const_cast<PropertyRef *>(this)->type_size(); }
        unsigned ptype() const { return type_size() & 0xf; }
        unsigned size() const { return type_size() >> 4; }
        uint8_t *val() const { return &_chunk[_offset + 3]; }

        PropertyList *&link()
        {
            assert(ptype() == p_link);
            assert(_offset == chunk_end() - sizeof (PropertyList *));
            return *(PropertyList **)(&_chunk[_offset+1]);
        }

        StringID &get_id()
        {
            assert(ptype() >= p_novalue && ptype() <= p_blob);
            assert(size() >= 2);
            return *(StringID *)(&_chunk[_offset+1]);
        }

        Property get_value() const;

        unsigned free_space() const;

        bool skip_to_next();
        void skip() { _offset += size() + 1; }
        bool next() { skip(); return skip_to_next(); }
        bool not_done() const
            { return _offset <= chunk_end() && ptype() != p_end; }

        void set_id(StringID id) { this->get_id() = id; }

        void set_size(unsigned old_size, unsigned new_size);
        void set_value(const Property &, unsigned size, Allocator &);
        void set_link(PropertyList *p_chunk, TransactionImpl *);
        void set_blob(const void *value, std::size_t size,
                      Allocator &allocator);
        void free(TransactionImpl *);
        void follow_link() { *this = PropertyRef(link()); }

        void make_space(PropertyRef &);
        void copy(const PropertyRef &);

        PropertyRef() : _chunk(0), _offset(0) { }
        explicit PropertyRef(const PropertyList *list)
            : _chunk((uint8_t *)list), _offset(1)
            {}
        PropertyRef(const PropertyRef &p, unsigned size)
            : _chunk(p._chunk), _offset(p._offset + size)
            { assert(_offset <= chunk_size()); }

    public:
        StringID id() const { return const_cast<PropertyRef *>(this)->get_id(); }
        operator Property() const { return get_value(); }

        PropertyType type() const
        {
            assert(ptype() >= p_novalue && ptype() <= p_blob);
            static const PropertyType type_map[] = {
                t_novalue, t_boolean, t_boolean, t_integer, t_float,
                t_time, t_string, t_string, t_blob
            };
            return type_map[ptype() - p_novalue];
        }

        bool bool_value() const;
        long long int_value() const;
        std::string string_value() const;
        double float_value() const;
        Time time_value() const;
        Property::blob_t blob_value() const;
    };

    typedef IteratorImplIntf<PropertyRef> PropertyIteratorImplIntf;
    class PropertyIterator : public Iterator<PropertyIteratorImplIntf> {
    public:
        explicit PropertyIterator(PropertyIteratorImplIntf *i)
            : Iterator<PropertyIteratorImplIntf>(i) { }

        PropertyIterator filter(std::function<Disposition(const Ref_type &)> f);
    };

    class PropertyList {
        static const unsigned chunk_size = 64;

        uint8_t _chunk0[];

        class PropertySpace;
        bool find_property(StringID property, PropertyRef &p,
                           PropertySpace *space = 0) const;
        bool find_space(PropertySpace &space, PropertyRef &start) const;
        void add_chunk(PropertyRef &end, TransactionImpl *, Allocator &);
        static unsigned get_space(const Property &);

    public:
        PropertyList(const PropertyList &) = delete;
        ~PropertyList() = delete;
        void operator=(const PropertyList &) = delete;
        void init(std::size_t size);
        bool check_property(StringID property, Property &result) const;
        Property get_property(StringID id) const;
        PropertyIterator get_properties() const;
        void set_property(StringID id, const Property &new_value,
                          Property &old_value);
        void remove_property(StringID name);
    };
};

namespace Jarvis {
    class Edge;
    class EdgeRef;

    class EdgeIteratorImplIntf : public IteratorImplIntf<EdgeRef> {
        friend class EdgeRef;
        virtual Edge *get_edge() const = 0;
        virtual StringID get_tag() const = 0;
        virtual Node &get_source() const = 0;
        virtual Node &get_destination() const = 0;
    };

    class EdgeRef {
        EdgeIteratorImplIntf *_impl;

        Edge *edge() const { return _impl->get_edge(); }

    public:
        EdgeRef(const EdgeRef &) = delete;
        void operator=(const EdgeRef &) = delete;
        EdgeRef(EdgeIteratorImplIntf *impl) : _impl(impl) {}
        operator Edge &() { return *edge(); }
        StringID get_tag() const { return _impl->get_tag(); }
        Node &get_source() const { return _impl->get_source(); }
        Node &get_destination() const { return _impl->get_destination(); }
        bool check_property(StringID id, Property &result) const;
        Property get_property(StringID id) const;
        PropertyIterator get_properties() const;
        void set_property(StringID id, const Property &property);
        void remove_property(StringID id);
    };

    class EdgeIterator : public Iterator<IteratorImplIntf<EdgeRef>> {
    public:
        explicit EdgeIterator(IteratorImplIntf<EdgeRef> *i)
            : Iterator<IteratorImplIntf<EdgeRef>>(i) { }

        EdgeIterator filter(const PropertyPredicate &pp);
        EdgeIterator filter(std::function<Disposition(const Ref_type &)> f);
    };
};

namespace Jarvis {
    class Path;
    class PathRef;

    class PathIteratorImplBaseIntf : public IteratorImplIntf<PathRef> {
    public:
        virtual NodeIterator end_nodes() const = 0;
    };

    class PathIteratorImplIntf : public PathIteratorImplBaseIntf {
        friend class PathRef;
        virtual Node &start_node() const = 0;
        virtual Node &end_node() const = 0;
        virtual int length() const = 0;
        virtual EdgeIterator get_edges() const = 0;
    };

    class PathRef {
        PathIteratorImplIntf *_impl;
    public:
        PathRef(const PathRef &) = delete;
        void operator=(const PathRef &) = delete;
        PathRef(PathIteratorImplIntf *i) : _impl(i) { }
        operator Path() const;
        Node &start_node() const { return _impl->start_node(); }
        Node &end_node() const { return _impl->end_node(); }
        int length() const { return _impl->length(); }
        EdgeIterator get_edges() const { return _impl->get_edges(); }
    };

    class PathIterator : public Iterator<PathIteratorImplBaseIntf> {
    public:
        explicit PathIterator(PathIteratorImplBaseIntf *i)
            : Iterator<PathIteratorImplBaseIntf>(i) { }

        PathIterator filter(std::function<Disposition(const Ref_type &)> f);

        NodeIterator end_nodes() const
            { if (!_impl) return NodeIterator(NULL); return _impl->end_nodes(); }
    };
};
