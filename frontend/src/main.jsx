// gets App.jsx and renders to the DOM

import { StrictMode } from 'react'
import { createRoot } from 'react-dom/client'
import './styles/index.css'
import App from './App.jsx'
fetch('/api/').then(r => r.text()).then(console.log) // should log "Hello world"

createRoot(document.getElementById('root')).render(
  <StrictMode>
    <App />
  </StrictMode>,
)
