#pragma once

namespace LuaCpp {

class State {
public:
    State() : State(false) {}
    State(const State &other) = delete;
    State &operator=(const State &other) = delete;

    State(bool should_open_libs) : 
        _l(nullptr), 
        _l_owner(true), 
        _exception_handler(new ExceptionHandler) {
        _l = luaL_newstate();
        if (should_open_libs) {
            luaL_openlibs(_l);
        }
        _registry.reset(new Registry(_l));
        HandleExceptionsPrintingToStdOut();
    }
    
    State(lua_State *l) : 
        _l(l), 
        _l_owner(false), 
        _exception_handler(new ExceptionHandler) {
        _registry.reset(new Registry(_l));
        HandleExceptionsPrintingToStdOut();
    }
    
    State(State &&other) : 
        _l(other._l), 
        _l_owner(other._l_owner), 
        _registry(std::move(other._registry)) {
        other._l = nullptr;
    }

    State &operator=(State &&other) {
        if (&other == this) {
            return *this;
        }
        _l = other._l;
        _l_owner = other._l_owner;
        _registry = std::move(other._registry);
        other._l = nullptr;
        return *this;
    }

    ~State() {
        if (_l != nullptr && _l_owner) {
            ForceGC();
            lua_close(_l);
        }
        _l = nullptr;
    }

    int Size() const {
        return lua_gettop(_l);
    }

    float Version() const {
        static int version;
        version = (int)(LUA_VERSION_NUM);
        return (float)(version / 100) + (float)(version % 10) / 10;
    }

    template <typename T>
    void RegisterSearcher(T callback) {
        _searchers.emplace_back(
            std::unique_ptr<Searcher>(new Searcher(_l, callback))
        );
    }

    template <typename T>
    void RegisterModule(const std::string &name, T callback) {
        _modules.emplace(
            std::make_pair(
                name,
                std::unique_ptr<Module>(new Module(_l, name, *_registry, *_exception_handler, callback))
            )
        );
    }

    void Require(const char *name);
    void Require(const char *name, lua_CFunction openf);

    void SetToNil(const char *name);

    bool LoadBuffer(const std::string &name, const char *code, size_t size = 0, const char *sandbox = "");
    bool Run();
    bool RunBuffer(const std::string &name, const char *code, size_t size = 0, const char *sandbox = "");

    bool Compile(const std::string &name, const char *text, std::string &code) const;

    void HandleExceptionsPrintingToStdOut() {
        *_exception_handler = ExceptionHandler([](int, std::string msg, std::exception_ptr) {
            _print(msg);
        });
    }

    void HandleExceptionsWith(ExceptionHandler::function handler) {
        *_exception_handler = ExceptionHandler(std::move(handler));
    }

    Selector operator[](const char *name) {
        return Selector(_l, nullptr, *_registry, *_exception_handler, name);
    }

    bool operator()(const char *code);

    void ForceGC() {
        lua_gc(_l, LUA_GCCOLLECT, 0);
    }

    void EnterInteractiveMode() {
        luaL_dostring(_l, "debug.debug()");
    }

    friend std::ostream &operator<<(std::ostream &os, const State &state);

private:
    static int _ldump_writer(lua_State *l, const void *p, size_t size, void *buff) {
        luaL_addlstring((luaL_Buffer *)buff, (const char *)p, size);
        return 0;
    }

    lua_State *_l;
    bool _l_owner;
    std::string _name;
    std::unique_ptr<Registry> _registry;
    std::unique_ptr<ExceptionHandler> _exception_handler;
    std::vector<std::unique_ptr<BaseSearcher>> _searchers;
    std::unordered_map<std::string, std::unique_ptr<BaseModule>> _modules;
};

inline std::ostream &operator<<(std::ostream &os, const State &state) {
    os << "LuaCpp::State - " << state._l;
    return os;
}

}
