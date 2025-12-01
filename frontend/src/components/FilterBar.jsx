// Filter Bar Selection

import React, { useState } from "react";

function FilterBar({  title, children }) 
{
    const [open, setOpen] = useState(false);

    return (
        <div>
            <button onClick={() => setOpen(!open)}>
            {title} {open ? "▲" : "▼"}
            </button>
            {open && <div>{children}</div>}
        </div>
    );
}

export default FilterBar