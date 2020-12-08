#include <tuple>

template<class L, class T>
concept BuilderCallable = requires (L l, T t) { l(t); };

template<class L>
class TupleBuilder
{
public:
    TupleBuilder(const L& l) : _l(l) {}

    template<class... Ts>
    constexpr auto operator() (Ts&&... args) const
    {
        std::tuple<> t;
        return Build(t, std::forward<Ts>(args)...);
    }

private:
    template<class... Us, class T, class... Ts> 
    constexpr auto Build(const std::tuple<Us...>& tpl, T&& t, Ts&&... args) const requires BuilderCallable<L, T>
    {
        auto _tpl = std::tuple_cat(tpl, std::make_tuple(_l(std::forward<T>(t))));
        return Build(_tpl, std::forward<Ts>(args)...);
    }

    template<class... Us, class T, class... Ts>
    constexpr auto Build(const std::tuple<Us...>& tpl, T&& t, Ts&&... args) const requires (!BuilderCallable<L, T>)
    {
     return Build(tpl, std::forward<Ts>(args)...);
    }

    template<class... Us, class T>
    constexpr auto Build(const std::tuple<Us...>& tpl, T&& t) const requires BuilderCallable<L,T>
    {
        return std::tuple_cat(tpl, std::make_tuple(_l(std::forward<T>(t))));
    }

    template<class... Us, class T>
    constexpr auto Build(const std::tuple<Us...>& tpl, const T& t) const requires (!BuilderCallable<L, T>)
    {
        return tpl;
    }

    L _l;
};

template<class L, class... Ts>
constexpr auto BuildFromOther(const L& l, const std::tuple<Ts...>& t)
{
    return std::apply(TupleBuilder(l), t);
}
