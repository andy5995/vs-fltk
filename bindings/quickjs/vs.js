//TODO: Move out from quicjs-js.cpp into here! Without #embed it is not that easy :(.
var $$ = (base, string) => {
    let self = (base === undefined ? this.core.self : this.core.vs_resolve_name_path(base, string));
    return {
        __self: self,
        $: (string) => this.core.vs_resolve_name_path(self, string),
        prop: (key, value) => this.core.vs_apply_prop(self, key, value),
        mixin: (value) => this.core.vs_apply_prop(self, "mixin", value),
        call: (name, ...args) => this.core.vs_call(self, name, ...args),
        //set
        //get
        log: (sev, string, ...args) => this.core.vs_log(sev, self, string, ...args)
    }
};

var $ = $$();

var $cb = (fn) => { globalThis[`__EXPORT_CB__${fn.name}`] = fn; };
var $callback = $cb;
var $plotter = (fn) => { globalThis[`__EXPORT_DRW_${fn.name}`] = fn; };
var $getter = (fn) => { globalThis[`__EXPORT_GET_${fn.name}`] = fn; };
var $setter = (fn) => { globalThis[`__EXPORT_SET_${fn.name}`] = fn; };
var $fn = (fn) => { globalThis[`__EXPORT_UKN_${fn.name}`] = fn; };
