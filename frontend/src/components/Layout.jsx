import NavBar from "./NavBar";
import { Outlet } from "react-router-dom";

function Layout({ loggedIn }) {

  return (
    <>
      <NavBar loggedIn={loggedIn} />
      <Outlet />
    </>
  );
}

export default Layout