module type Config = {
  type state;
  type action;
  let getInitialState: unit => state;
  let reducer: (state, action) => state;
};

module Make = (Config: Config) => {
  include Config;

  let state = ref(getInitialState());
  let listeners = ref([]);

  let dispatch = action => {
    state := reducer(state^, action);
    listeners^ |> List.iter(listener => listener());
  };

  let getState = () => state^;

  let subscribe = fn => {
    listeners := [fn, ...listeners^];
    () => listeners := listeners^ |> List.filter(l => l !== fn);
  };

  let useMappedState = mapper => {
    let (syncedState, setSyncedState) =
      React.useState(() => mapper(getState()));

    React.useEffect(() =>
      Some(subscribe(() => setSyncedState(_ => mapper(state^))))
    );

    syncedState;
  };

  let useState = () => useMappedState(state => state);

  module Consumer = {
    [@react.component]
    let make = (~mapper: state => 'a, ~children) => {
      let state = useMappedState(mapper);
      <> {children(state)} </>;
    };
  };
};