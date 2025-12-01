import React from "react";

import '../styles/checkbox.css'

function MultiCheckbox({ title, options, selected, setSelected }) {
  const toggle = (value) => {
    if (selected.includes(value)) {
      setSelected(selected.filter((v) => v !== value));
    } else {
      setSelected([...selected, value]);
    }
  };

  return (
    <div className="options-list">
      {options.map((opt) => (
        <label key={opt}>
          <input
            type="checkbox"
            checked={selected.includes(opt)}
            onChange={() => toggle(opt)}
          />
          {opt}
        </label>
      ))}
    </div>
  );
}

export default MultiCheckbox;
