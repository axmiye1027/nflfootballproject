// homepage - first page to appear when opened

import { useState } from 'react'
import reactLogo from './assets/react.svg'
import viteLogo from '/vite.svg'
import './styles/App.css'

function App() {
  const [count, setCount] = useState(0)

  return (
    <>
      {/* NAVIGATION */}
      <div className="row">
        <a className="nav-btn" href="https://vitejs.dev" target="_blank" rel="noreferrer">
          <img className="nav-img" alt="Vite logo" src={viteLogo} />
        </a>

        <a className="nav-btn" href="https://react.dev" target="_blank" rel="noreferrer">
          <img className="nav-btn" alt="React logo" src={reactLogo} />
        </a>

        <a className="nav-btn" href="https://react.dev" target="_blank" rel="noreferrer">
          <img className="nav-btn" alt="React logo" src={reactLogo} />
        </a>
      </div>
        
      <img className="nav-btn" alt="search button"
          src="https://preview.redd.it/why-does-scootaloos-cutie-mark-have-a-wing-v0-ruz6lnq3z64c1.png?width=885&format=png&auto=webp&s=e151e0d62c13c97e5fab681a9a2077f38d93117f" />
        
        <img className="nav-btn" alt="admin button"
          src="https://derpicdn.net/img/2016/8/9/1221208/full.png" />

        <img className="nav-btn" alt="search button"
          src="https://derpicdn.net/img/2016/4/23/1138655/large.png" />

      {/* HEADER */}
      <div className="title">List of Teams</div>

      {/* SEARCH - ICON - FILTERS */}
      <div className="row">
        
      </div>

      {/* TEAM LIST */}
    </>
  )
}

export default App
