entity hsm is
  port (
    clk: in std_logic;
    reset: in std_logic;
    event: in std_logic;
    payload: in std_logic_vector(7 downto 0);
    state: out std_logic_vector(1 downto 0)
  );
end entity;

architecture rtl of hsm is
  type state_type is (state_a, state_b);
  signal current_state: state_type;
  signal next_state: state_type;
  signal payload_data: std_logic_vector(7 downto 0);
begin
  -- State machine process
  process(clk, reset)
  begin
    if (reset = '1') then
      current_state <= state_a;
    elsif (clk'event and clk = '1') then
      current_state <= next_state;
    end if;
  end process;

  -- State machine next state logic
  process(current_state, event, payload_data)
  begin
    case current_state is
      when state_a =>
        if (event = '1') then
          next_state <= state_b;
          payload_data <= payload;
        else
          next_state <= state_a;
        end if;
      when state_b =>
        if (event = '1') then
          next_state <= state_a;
          payload_data <= payload;
        else
          next_state <= state_b;
        end if;
      when others =>
        next_state <= state_a;
    end case;
  end process;

  -- Outputs
  state <= current_state;
end architecture;
