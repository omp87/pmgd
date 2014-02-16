#pragma once

#include "iterator.h"

namespace Jarvis {
    template <typename B, typename F>
    class IteratorFilter : public B::Impl_type {
    protected:
        B *base_iter;
        F func;

    private:
        bool _done;

        virtual bool _next() {
            while (bool(*base_iter)) {
                switch (func(*base_iter)) {
                    case dont_pass: base_iter->next(); break;
                    case pass: return true;
                    case stop: base_iter->done(); return false;
                    case pass_stop: _done = true; return true;
                    default: throw e_not_implemented;
                }
            }
            return false;
        }

    public:
        IteratorFilter(B *i, F f) : base_iter(i), func(f) { _next(); }
        operator bool() const { return bool(*base_iter); }
        const typename B::Ref_type &operator*() const
            { return (*base_iter).operator*(); }
        const typename B::Ref_type *operator->() const
            { return (*base_iter).operator->(); }
        typename B::Ref_type &operator*() { return (*base_iter).operator*(); }
        typename B::Ref_type *operator->() { return (*base_iter).operator->(); }

        bool next()
        {
            if (_done) {
                base_iter->done();
                return false;
            }
            else {
                base_iter->next();
                return _next();
            }
        }
    };

    template <typename Iter> class PropertyFilter {
        const PropertyPredicate &pp;
    public:
        PropertyFilter(const PropertyPredicate &p) : pp(p) { }
        Disposition operator()(const Iter &);
    };

    template <typename F>
    class NodeIteratorFilter : public IteratorFilter<NodeIterator, F> {
    public:
        NodeIteratorFilter(NodeIterator *i, F f)
            : IteratorFilter<NodeIterator, F>(i, f) { }
    };

    template <typename F>
    class NodeIteratorPropertyFilter : public IteratorFilter<NodeIterator, F> {
    public:
        NodeIteratorPropertyFilter(NodeIterator *i, const PropertyPredicate &pp)
            : IteratorFilter<NodeIterator, F>(i, PropertyFilter<NodeIterator>(pp)) { }
    };

    template <typename F>
    class EdgeIteratorFilter : public IteratorFilter<EdgeIterator, F> {
    public:
        EdgeIteratorFilter(EdgeIterator *i, F f)
            : IteratorFilter<EdgeIterator, F>(i, f) { }
    };

    template <typename F>
    class EdgeIteratorPropertyFilter : public IteratorFilter<EdgeIterator, F> {
    public:
        EdgeIteratorPropertyFilter(EdgeIterator *i, const PropertyPredicate &pp)
            : IteratorFilter<EdgeIterator, F>(i, PropertyFilter<EdgeIterator>(pp)) { }
    };

    template <typename F>
    class PropertyIteratorFilter : public IteratorFilter<PropertyIterator, F> {
    public:
        PropertyIteratorFilter(PropertyIterator *i, F f)
            : IteratorFilter<PropertyIterator, F>(i, f) { }
    };

    template <typename F>
    class PathIteratorFilter : public IteratorFilter<PathIterator, F> {
        using IteratorFilter<PathIterator, F>::base_iter;

    public:
        PathIteratorFilter(PathIterator *i, F f)
            : IteratorFilter<PathIterator, F>(i, f) { }

        NodeIterator end_nodes() const { return base_iter->end_nodes(); }
    };

    inline NodeIterator NodeIterator::filter(const PropertyPredicate &pp)
        { return NodeIterator(new NodeIteratorPropertyFilter<
                     PropertyFilter<NodeIterator> >(this, pp)); }

    template <typename F> inline NodeIterator NodeIterator::filter(F f)
        { return NodeIterator(new NodeIteratorFilter<F>(this, f)); }

    inline EdgeIterator EdgeIterator::filter(const PropertyPredicate &pp)
        { return EdgeIterator(new EdgeIteratorPropertyFilter<
                     PropertyFilter<EdgeIterator> >(this, pp)); }

    template <typename F> inline EdgeIterator EdgeIterator::filter(F f)
        { return EdgeIterator(new EdgeIteratorFilter<F>(this, f)); }

    template <typename F> inline PropertyIterator PropertyIterator::filter(F f)
        { return PropertyIterator(new PropertyIteratorFilter<F>(this, f)); }

    template <typename F> inline PathIterator PathIterator::filter(F f)
        { return PathIterator(new PathIteratorFilter<F>(this, f)); }
};
