// homepage / home button - first page to appear when opened

import { useState } from 'react'
//import { BrowserRouter, Link, Route, Routes } from 'react-router-dom'
import './styles/App.css'

// components
import NavBar from './components/NavBar.jsx'
import SearchBar from './components/SearchBar.jsx'
import TeamInfo from './components/TeamInfo.jsx'
// data
//import teams from './data/teams.json'
//import LoginPage from "./pages/login.jsx"

function App() {
  const [count, setCount] = useState(0)

  // return (
  //   <BrowserRouter>
  //     <Link to="/login"></Link>
  //     <Routes>
  //        <Route path="/" element={<h1>BADADAD</h1>} />
  //       <Route path="/login" element={<LoginPage />} />
  //     </Routes> 
  //   </BrowserRouter>
  // )

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
{/* 
      <Routes>
         <Route path="/" element={<h1>BADADAD</h1>} />
        <Route path="/login" element={<LoginPage />} />
      </Routes> */}
    </>
  )
}

export default App
