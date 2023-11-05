
class HierarchicalStatemachineTransient extends HTMLElement {
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
                <!-- Add your HTML markup here -->
            </div>
        `;
    }
}

customElements.define('hierarchical-statemachine-transient', HierarchicalStatemachineTransient);
