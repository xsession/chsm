
class InitStateComponent extends HTMLElement {
    constructor() {
        super();
        this.attachShadow({ mode: 'open' });
    }

    connectedCallback() {
        this.render();
    }

    render() {
        this.shadowRoot.innerHTML = `
            <style>
                /* Add your custom styles here */
            </style>
            <div>
                <h2>Init State</h2>
                <p>This is the initial state of the hierarchical state machine.</p>
            </div>
        `;
    }
}

customElements.define('init-state', InitStateComponent);
