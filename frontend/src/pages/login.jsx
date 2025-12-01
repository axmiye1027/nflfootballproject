// Login Popup Page
import React, { useState } from 'react'

function LoginPage({ onSuccess } = {}) {
    // STYLES
    const overlay = {
        position: 'fixed',
        inset: 0,
        display: 'flex',
        alignItems: 'center',
        justifyContent: 'center',
        backgroundColor: 'rgba(0,0,0,0.45)',
        zIndex: 1000
    }

    const panel = {
        backgroundColor: 'var(--pink)',
        padding: '1rem',
        border: 'var(--border)',
        borderRadius: '6px',
        boxShadow: '0 6px 18px rgba(0,0,0,0.2)',
        minWidth: '280px'
    }

    const [username, setUsername] = useState('')
    const [password, setPassword] = useState('')
    const [loading, setLoading] = useState(false)
    const [error, setError] = useState(null)

    const handleSubmit = async (e) => {
        e.preventDefault()
        setError(null)
        if (!username || !password) {
            setError('Please enter username and password')
            return
        }
        setLoading(true)
        try {
            const res = await fetch('http://localhost:18080/login', {
                method: 'POST',
                credentials: 'include',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ username, password }),
            })
            if (!res.ok) {
                const txt = await res.text().catch(() => null)
                throw new Error(txt || `HTTP ${res.status}`)
            }
            const data = await res.json().catch(() => ({}))
            // expected { isAdmin: boolean, user: {...} }
            if (onSuccess) onSuccess({ isAdmin: !!data.isAdmin, user: data.user })
        } catch (err) {
            setError(err.message || 'Login failed')
        } finally {
            setLoading(false)
        }
    }

    return (
        <div style={overlay}>
            <div style={panel}>
                <h2>Admin Login</h2>
                <form onSubmit={handleSubmit}>
                    <div>
                        <label htmlFor="login-username">Username</label>
                        <input id="login-username" value={username} onChange={(e) => setUsername(e.target.value)} />
                    </div>
                    <div style={{ marginTop: '.5rem' }}>
                        <label htmlFor="login-password">Password</label>
                        <input id="login-password" type="password" value={password} onChange={(e) => setPassword(e.target.value)} />
                    </div>
                    {error && <div style={{ color: 'red', marginTop: '.5rem' }}>{error}</div>}
                    <div>
                        <button type="submit" disabled={loading}>{loading ? 'Signing in...' : 'Sign in'}</button>
                    </div>
                </form>
            </div>
        </div>
    )
}

export default LoginPage;
