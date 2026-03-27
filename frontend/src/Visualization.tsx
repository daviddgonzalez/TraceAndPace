function Visualization() {
  return (
    <>
      <div className="card" style={{ width: "18rem" }}>
        <div className="card-body">
          <div className="d-flex justify-content-between">
            <h5 className="card-title">{"Tree Name"}</h5>
            <button
              type="button"
              className="btn-close"
              aria-label="Close"
            ></button>
          </div>
          <p className="card-text">
            This is where a tree visualization will be
          </p>
        </div>
      </div>
    </>
  );
}

export default Visualization;
