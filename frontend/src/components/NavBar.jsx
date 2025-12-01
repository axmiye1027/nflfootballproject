import { useState } from 'react'
import { Link } from 'react-router-dom'

// NavBar componentS
function NavBar({ setShowLogin, auth, setShowAdmin }) {
    // STYLES
    const navStyle = {
        position: 'sticky',
        top: '0px',
        left: '0px',
        width: '100%',
        maxHeight: '5em',
        overflow: 'hidden',
        display: 'flex',
        justifyContent: 'right',
        alignItems: 'center',
        borderBottom: 'var(--border)',
        background: 'linear-gradient(to right, var(--red), var(--pink), var(--purple))'
    };

    const navLogoStyle = {
        marginRight: 'auto',
        height: '12em',
        willChange: 'filter',
        transition: 'filter 300ms'
    };

    const navLinkStyle = {
        display: 'flex',
        flexDirection: 'column',
        alignItems: 'center',
        padding: '1em',
        transition: 'background-color 0.25s'
    };

    const navImgStyle = {
        height: '2.5em',
        willChange: 'filter',
        transition: 'filter 300ms'
    };

    const navTextStyle = {
        fontWeight: 'bold',
        fontSize: '1.2em',
        fontFamily: 'var(--header)',
        color: 'var(--purpleNeon)',
        textShadow: '1.5px 1.5px 0 var(--purpleDark), -1.5px 1.5px 0 var(--purpleDark), -1.5px -1.5px 0 var(--purpleDark), 1.5px -1.5px 0 var(--purpleDark)',
    };

    // DISPLAY

    return (
        <nav style={navStyle}>
            <div className="row" style={{ width: '100%', justifyContent: 'space-between' }}>

                {/* PONY IMAGES */}
                <div className="row">
                    <img style={navLogoStyle} alt="ponies"
                        src="https://derpicdn.net/img/2020/1/18/2250488/large.png" />
                </div>
                {/* END PONY IMAGES */}
                

                {/* BUTTONS */} 
                <div className="row" style={{ justifyContent: 'flex-end' }}>
                    {/* CREATE TRIP BUTTON */}
                    <Link style={navLinkStyle} to="/trip">
                        <img style={navImgStyle} alt="search button"
                            src="https://derpicdn.net/img/2016/8/9/1221208/full.png" />
                        <span style={navTextStyle}>Trip</span>
                    </Link>

                    {/* HOME BUTTON */}
                    <Link style={navLinkStyle} to="/">
                        <img style={navImgStyle} alt="search button"
                            src="https://images-wixmp-ed30a86b8c4ca887773594c2.wixmp.com/f/a3257d96-538a-4324-b870-242e0de13df4/d9cp787-363a4167-0d94-4a9b-8458-cafb05174321.png/v1/fill/w_400,h_408/applebloom_cutie_mark_by_captsteamclank_d9cp787-fullview.png?token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJzdWIiOiJ1cm46YXBwOjdlMGQxODg5ODIyNjQzNzNhNWYwZDQxNWVhMGQyNmUwIiwiaXNzIjoidXJuOmFwcDo3ZTBkMTg4OTgyMjY0MzczYTVmMGQ0MTVlYTBkMjZlMCIsIm9iaiI6W1t7ImhlaWdodCI6Ijw9NDA4IiwicGF0aCI6IlwvZlwvYTMyNTdkOTYtNTM4YS00MzI0LWI4NzAtMjQyZTBkZTEzZGY0XC9kOWNwNzg3LTM2M2E0MTY3LTBkOTQtNGE5Yi04NDU4LWNhZmIwNTE3NDMyMS5wbmciLCJ3aWR0aCI6Ijw9NDAwIn1dXSwiYXVkIjpbInVybjpzZXJ2aWNlOmltYWdlLm9wZXJhdGlvbnMiXX0.3yYxnh2m-GnyEcgCk7rK9m7HtjsJBQ2NG8lBNHK-xQU" />
                        <span style={navTextStyle}>Home</span>
                    </Link>

                    {/* LOGIN BUTTON */}
                    <button
                        onClick={() => setShowLogin && setShowLogin((v) => !v)}
                        aria-pressed={false}
                        style={{
                            ...navLinkStyle,
                            background: 'none',
                            border: 'none',
                            padding: 0,
                            cursor: 'pointer'
                        }}
                    >
                        <img style={navImgStyle} alt="login button"
                            src="https://preview.redd.it/why-does-scootaloos-cutie-mark-have-a-wing-v0-ruz6lnq3z64c1.png?width=885&format=png&auto=webp&s=e151e0d62c13c97e5fab681a9a2077f38d93117f" />
                        <span style={navTextStyle}>Login</span>
                    </button>

                    {/* Admin preview toggle (dev helper) */}
                    {typeof window !== 'undefined' && sessionStorage.getItem('isAdmin') === 'true' ? (
                        <Link style={navLinkStyle} to="/" onClick={() => sessionStorage.removeItem('isAdmin')}>
                            <span style={navTextStyle}>Exit Admin Preview</span>
                        </Link>
                    ) : (
                        <Link style={navLinkStyle} to="/admin" onClick={() => sessionStorage.setItem('isAdmin', 'true')}>
                            <span style={navTextStyle}>Admin Preview</span>
                        </Link>
                    )}
                </div>
                {/* END BUTTONS */}
                
            </div>
            {/* Login panel is rendered by App when `showLogin` state is true. */}
        </nav>
    );
}
// END NavBar component

export default NavBar;