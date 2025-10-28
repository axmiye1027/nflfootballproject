// homepage - first page to appear when opened

import { useState } from 'react'
import './styles/App.css'

// components
import NavBar from './components/NavBar.jsx'
import SearchBar from './components/SearchBar.jsx'
import TeamInfo from './components/TeamInfo.jsx'
// data
//import teams from './data/teams.json'

function App() {
  const [count, setCount] = useState(0)

  return (
    <>
      {/* NAVIGATION */}
      <NavBar />

      {/* HEADER */}
      <div className="title" style={{ 
        backgroundSize: 'cover',
        backgroundPosition: 'center',
        backgroundImage: `url('')` }}>List of Teams</div>

      {/* SEARCH - ICON - FILTERS */}
      <div className="row">

        {/* SEARCH BAR */}
        <SearchBar />

        {/* LOGO */}

        {/* FILTERS */}
        <select>
          <option value="">All Teams</option>
          <option value="AFC">AFC</option>
          <option value="NFC">NFC</option>
        </select>

      </div>
      {/* END SEARCH - ICON - FILTERS */}


      {/* TEAM LIST */}
      <TeamInfo />
      {/* END TEAM LIST */}
    </>
  )
}

export default App
