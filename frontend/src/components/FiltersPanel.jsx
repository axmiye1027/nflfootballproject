import React         from 'react';
import FilterBar     from './FilterBar';
import MultiCheckbox from './MultiCheckbox';

function FiltersPanel({ filters, updateFilter }) {
  return (
    <div className="filters">
      <FilterBar title="Conference">
        <MultiCheckbox
          options={["AFC", "NFC"]}
          selected={filters.conference}
          setSelected={v => updateFilter("conference", v)}
        />
      </FilterBar>

      <FilterBar title="Division">
        <MultiCheckbox
          options={[
            "AFC East", "AFC North", "AFC South", "AFC West",
            "NFC East", "NFC North", "NFC South", "NFC West",
          ]}
          selected={filters.divisions}
          setSelected={v => updateFilter("divisions", v)}
        />
      </FilterBar>

      <FilterBar title="Roof Type">
        <MultiCheckbox
          options={["Open", "Fixed", "Retractable"]}
          selected={filters.roofTypes}
          setSelected={v => updateFilter("roofTypes", v)}
        />
      </FilterBar>
    </div>
  );
}

export default React.memo(FiltersPanel);
