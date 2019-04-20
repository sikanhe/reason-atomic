# Atomic [![npm version](https://img.shields.io/npm/v/reason-atomic.svg)](https://www.npmjs.com/package/reason-atomic)

Dead simple shared state in Reason React with a redux-like interface.

# Installation 

```bash
yarn add reason-atomic
``` 

Then add `reason-atomic` to your `bs-dependencies` in `bsconfig.json`

```
{
  ...
  "bs-dependencies": ["reason-atomic"]
}

```

# Usage

Configure your state by creating a module with `state`, `getInitialState`, `action`, and `reducer`.


```reason 
module Config = {
  type state = {count: int};

  let getInitialState = () => {count: 0};

  type action =
    | Increment
    | Decrement
    | NoChange;

  let reducer = state =>
    fun
    | Increment => {count: state.count + 1}
    | Decrement => {count: state.count - 1}
    | NoChange => state;

}; 
```

Create a new state module with the config

```reason
module AppState = Atomic.Make(Config);
// module AppState2 = Atomic.Make(Config);
// module AppState3 = Atomic.Make(Config);

// Create as many instances as you like..
```

Access the state as a hook, and dispatch actions 

```reason 
[@react.component]
let make = () => {
  let state = AppState.useState();

  <div>
    {React.string("global count: " ++ string_of_int(state.count))}
    <button onClick={_ => AppState.dispatch(Increment)}> {React.string("+")} </button>
    <button onClick={_ => AppState.dispatch(Decrement)}> {React.string("-")} </button>
  </div>;
};
```

If you only care about a subset of the state, you can use `useMappedState`. This also means that
the component with the hook won't re-render unless the state it cares about changes - avoiding excessive re-rendering.

```reason 
[@react.component]
let make = () => {
  let count = AppState.useMappedState(state => state.count);

  <div>
    {React.string("global count: " ++ string_of_int(count))}
  </div>;
};
```

You can also use the render-props pattern to consume the state as well, using the `Consumer` 
component and pass the `mapper` prop.

```reason
[@react.component]
let make = () => {
  <AppState.Consumer mapper={state => state.count}>
    {count => React.string("global count: " ++ string_of_int(count))}
  </AppState.Consumer>;
};
```
